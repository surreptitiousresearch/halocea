// apLogDV @ 0x825C94E0 — ap_log.cpp
// Debug-output variant of apLogV: same prefix + format handling, but the finished line (with a
// trailing newline appended) is sent to the platform debug-output channel (_osOutputDebugString)
// instead of the log callbacks. A null `text` still flushes the callbacks with a null line.
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsTSTRING_BUF_HEADER.h"
#include <stdio.h>
#include <string.h>

extern int gs_block;                 // boundary — when set, all logging is suppressed
extern dsTSTRING<char> gs_prefix;    // boundary — active log-line prefix ("" = none)

extern void CallLogCB(const char *text);          // 0x825C86E0 — callback dispatch (null-flush path)
extern void _osOutputDebugString(const char *s);  // boundary — platform debug output channel

void apLogDV(char *text, char *argPtr)
{
    if (gs_block)
        return;

    if (!text)
    {
        CallLogCB(nullptr);
        return;
    }

    char *str = text;
    if (gs_prefix.pBuffer->strLen)
    {
        // See apLogV: lazily-initialised shared prefix scratch (binary inlines the guard +
        // UnsafeInitEmpty).
        static dsTSTRING<char> tmpString;
        static bool tmpInited = false;
        if (!tmpInited)
        {
            tmpInited = true;
            tmpString.pBuffer = nullptr;
            tmpString.UnsafeInitEmpty();
        }
        tmpString = gs_prefix;
        tmpString.Insert(tmpString.pBuffer->strLen, str, -1);
        str = tmpString.pBuffer->str;
    }

    char line[1072];
    vsnprintf(line, 0x400, str, argPtr);
    strcat(line, "\n"); // binary appends the newline+NUL by hand at the string end
    _osOutputDebugString(line);
}
