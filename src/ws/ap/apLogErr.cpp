// apLogErr @0x825C8CF8 — ap_log.cpp
// Variadic error-log sink (the error-severity counterpart to apLogV). Formats `text` with the
// varargs into a 1KB line buffer, dispatches it through the registered error callbacks
// (CallLogErrCB) and, when present, the console log sink (_apConCb). A null `text` flushes the
// error callbacks with a null line.
//
// DEVIATION: the raw decompile mis-modelled this as a fixed-arg __int64 function and read the
// format pointer out of HIDWORD(text) — the usual PPC vararg mis-read. It is a plain
// `(const char *text, ...)` variadic; the `HIDWORD(text)` test/format-arg is just `text`.
#include "../../headers/ws/ap/apLOG_CON_CB.h"
#include <stdio.h>
#include <stdarg.h>

extern apLOG_CON_CB *_apConCb;              // boundary — optional console log sink (null = none)
extern void CallLogErrCB(const char *text); // 0x825C87A8 — dispatch a line to all error callbacks (boundary)

void apLogErr(const char *text, ...)
{
    char line[1024];

    if (text)
    {
        va_list va;
        va_start(va, text);
        vsnprintf(line, 0x400, text, va);
        va_end(va);

        CallLogErrCB(line);
        if (_apConCb)
            _apConCb->AddLogMessage(line);
    }
    else
    {
        CallLogErrCB(nullptr);
    }
}
