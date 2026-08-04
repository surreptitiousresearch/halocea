// STRONG_ASSERT_DUMMY::Crash @0x825202A8 — the ws-engine "strong assertion" terminal handler.
// Fires at most once (guarded by the module-global bHappend). Formats the failed condition,
// source location and caller-supplied info into a message (optionally appending the thread-local
// context string carried in the shared empty-string static), prints it, pops a message box when on
// the main thread outside build mode, breaks into the debugger, and emits the line to the debug
// output. In build mode it deliberately dereferences a null `this->dummy` to force the crash.
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsTSTRING_BUF_HEADER.h"
#include <stdio.h>

namespace STRONG_ASSERT_DUMMY {

struct DUMMY { int dummy; };

extern bool bHappend;        // one-shot guard: crash handler runs only the first time
extern bool m_bBuildMode;    // build/tool mode: suppress msgbox+debugbreak, force null-deref crash

// boundary — os/ap terminal-handler surface
extern "C" {
    int  osGetCurThreadId(void);
    int  osGetMainThreadId(void);
    void osDebugBreak(void);
    void osOutputDebugString(const char *fmt, ...);
    void _apMsgBox(const char *text);
    void dlFree(void *block);
}

void Crash(DUMMY *self, const char *condition, const char *file, int line, const char *info)
{
    if (bHappend)
        return;
    bHappend = true;

    // shared, lazily-initialised empty string carries an optional thread-local suffix.
    static dsTSTRING<char> contextStr; // dsTSTRING<char>::UnsafeInitEmpty's magic-static emptyStr
    static bool contextInited = false;
    if (!contextInited)
    {
        contextInited = true;
        contextStr.pBuffer = nullptr;
        contextStr.pBuffer = contextStr.AllocBuffer(0, 0); // decompiler scope-qualified artifact -> member call (2026-07-31)
        // (shipped code registers an atexit destructor for the static here)
    }

    dsTSTRING_BUF_HEADER<char> *pBuffer = contextStr.pBuffer;
    ++pBuffer->refCount;

    char message[4176];
    if (pBuffer->strLen)
        snprintf(message, 0x1000,
                 "STRONG ASSERTION FAILED\nCrash coming...\n\n%s(%d): %s\n%s%s\n",
                 file, line, condition, info, pBuffer->str);
    else
        snprintf(message, 0x1000,
                 "STRONG ASSERTION FAILED\nCrash coming...\n\n%s(%d): %s\n%s\n",
                 file, line, condition, info);

    printf("%s\n", message);

    if (osGetCurThreadId() == osGetMainThreadId() && !m_bBuildMode)
        _apMsgBox(message);

    message[1024] = '\0';
    if (!m_bBuildMode)
        osDebugBreak();
    osOutputDebugString("%s\n", message);
    if (!m_bBuildMode)
        self->dummy = 0; // deliberate null-deref crash (callers pass self=nullptr) outside build mode

    if (pBuffer->refCount-- == 1)
        dlFree(pBuffer);
}

} // namespace STRONG_ASSERT_DUMMY
