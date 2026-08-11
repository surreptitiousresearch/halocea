#include "../../headers/ws/gs/gsSTRINGS.h"

// @0x826B4DC8 — copy `text` into `buf` (up to bufLen-1 chars, NUL-terminated), widening each byte.
void gsSTRINGS::AsciiToUnicode(const char *text, wchar_t *buf, int bufLen)
{
    wchar_t *dst = buf;
    while (*text && dst < &buf[bufLen - 1])
        *dst++ = *text++;
    *dst = 0;
}
