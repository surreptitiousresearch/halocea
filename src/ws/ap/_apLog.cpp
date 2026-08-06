// _apLog @0x825C96E0 — ap_log.cpp
// printf-style front-end for the ws-engine log sink. Unless the `logs` debug variable disables
// logging, format the va-args through the core sink apLogV. (The shipped code spills the first
// several integer/float argument registers to the outgoing arg-save area before the va_start —
// the PPC varargs home-area prologue — then simply forwards the va_list.)
#include "../../headers/ws/wb/dbgVAR_boundary.h"
#include <stdarg.h>

extern void apLogV(char *text, char *argPtr);      // 0x825C9390 — core variadic log sink
extern "C" dbgVAR_SIMPLE<bool, 1> dbg_logs;            // ap-log `logs` debug var (value != 0 -> suppress)

void _apLog(const char *fmt, ...)
{
    if (dbg_logs.value)
        return;

    va_list va;
    va_start(va, fmt);
    apLogV((char *)fmt, (char *)va);
    va_end(va);
}
