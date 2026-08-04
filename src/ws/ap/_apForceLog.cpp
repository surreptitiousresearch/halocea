// _apForceLog @0x825C9030 — ap_log.cpp
// printf-style front-end over apForceLogV: format the variadic message into (or manage) the named
// log file. Returns apForceLogV's status (1 = ok, 0 = failure/empty-name/too-large).
#include <stdarg.h>

extern "C" int apForceLogV(const char *name, char *string, char *argPtr); // 0x825C8D90

int _apForceLog(const char *name, char *string, ...)
{
    va_list va;
    va_start(va, string);
    int rc = apForceLogV(name, string, (char *)va);
    va_end(va);
    return rc;
}
