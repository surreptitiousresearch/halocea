#include "../../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsTSTRING_BUF_HEADER.h"

// STRONG_ASSERT_DUMMY::Crash @ 0x825202A8 — the Saber strong-assertion crash reporter, and the
// concrete body behind the hcex bridge's flattened extern STRONG_ASSERT_DUMMY_Crash.
//
// Fires at most once (bHappend latch). Builds a message from the failed condition, source
// location and caller-supplied info (optionally suffixed with the shared assert-context string),
// prints it, pops a message box on the main thread, breaks into the debugger, and in release
// builds stores 0 through `this` to force a fault. In build/tool mode all of the interactive/
// fault steps are skipped.

// --- boundary externs (OS / CRT / allocator / shared empty-string) -----------------------------
extern "C" int  bHappend;                                  // one-shot latch @ 0x841DB160
// DEVIATION: this object was named `dsStrongAssertMessage` here, colliding with the corpus's
// alias for the shared "" literal (.rdata @0x8200155A). It is neither: the disassembly at
// 0x825202E4 loads ?emptyStr@?1??UnsafeInitEmpty@?$dsTSTRING@D@@IAAXXZ@4V2@A (@0x841C3380) --
// the function-local static dsTSTRING<char> of dsTSTRING<char>::UnsafeInitEmpty(), inlined here.
// CAVEAT: its local-static guard (??_B?1??UnsafeInitEmpty@?$dsTSTRING@D@@IAAXXZ@51 @0x841C3384)
// and the guarded first-use AllocBuffer are not modelled here.
extern dsTSTRING<char> dsTSTRING_char_emptyStr;        // shared assert-context string (empty by default)
extern "C" int  _snprintf_0(char *buf, unsigned int size, const char *fmt, ...);
extern "C" int  printf(const char *fmt, ...);
extern int  osGetCurThreadId();
extern int  osGetMainThreadId();
extern void _apMsgBox(const char *text);
extern void osDebugBreak();
extern "C" void osOutputDebugString(const char *fmt, ...);
extern "C" void dlFree(void *ptr);

void STRONG_ASSERT_DUMMY::Crash(const char *condition, const char *file, int line, const char *info)
{
    if (bHappend)
        return;
    bHappend = 1;

    // Grab a ref on the shared assert-context string; when non-empty it is appended to the report.
    dsTSTRING_BUF_HEADER<char> *ctx = dsTSTRING_char_emptyStr.pBuffer;
    ++ctx->refCount;

    char message[4176];
    if (ctx->strLen)
        _snprintf_0(message, 0x1000, "STRONG ASSERTION FAILED\nCrash coming...\n\n%s(%d): %s\n%s%s\n",
                   file, line, condition, info, ctx->str);
    else
        _snprintf_0(message, 0x1000, "STRONG ASSERTION FAILED\nCrash coming...\n\n%s(%d): %s\n%s\n",
                   file, line, condition, info);

    printf("%s\n", message);

    if (osGetCurThreadId() == osGetMainThreadId() && !STRONG_ASSERT_DUMMY::m_bBuildMode)
        _apMsgBox(message);

    message[1024] = 0;

    if (!STRONG_ASSERT_DUMMY::m_bBuildMode)
        osDebugBreak();

    osOutputDebugString("%s\n", message);

    if (!STRONG_ASSERT_DUMMY::m_bBuildMode)
        this->dummy = 0; // release-build forced fault (null-store through the caller's this)

    if (ctx->refCount-- == 1)
        dlFree(ctx);
}
