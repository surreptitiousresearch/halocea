// apLogV @ 0x825C9390 — ap_log.cpp
// Core variadic log sink. When logging is not blocked (gs_block) and `text` is non-null, prepend
// the active global prefix (gs_prefix) if one is set, format with argPtr, then fan the line out to
// the registered log callbacks (CallLogCB) and, if present, the console log sink (_apConCb). A null
// `text` flushes the callbacks with a null line.
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsTSTRING_BUF_HEADER.h"
#include "../../headers/ws/ap/apLOG_CON_CB.h"
#include <stdio.h>

extern "C" int gs_block;                 // boundary — when set, all logging is suppressed
extern "C" dsTSTRING<char> gs_prefix;    // boundary — active log-line prefix ("" = none)
extern "C" apLOG_CON_CB *_apConCb;       // boundary — optional console log sink (null = none)

extern "C" void CallLogCB(const char *text); // 0x825C86E0 — dispatch a formatted line to all callbacks

void apLogV(char *text, char *argPtr)
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
        // build "<prefix><text>" in a lazily-initialised shared scratch string.
        // DEVIATION: the binary uses a hand-rolled one-shot guard around an inlined
        // dsTSTRING::UnsafeInitEmpty (whose own emptyStr magic-static is also inlined); expressed
        // here as a function-static string initialised via the UnsafeInitEmpty primitive.
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
    CallLogCB(line);
    if (_apConCb)
        _apConCb->AddLogMessage(line);
}
