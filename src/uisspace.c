/* uisspace @0x8376D6A0 — wide-character whitespace test (CRT iswctype with the _SPACE mask). */

#include <stddef.h>   /* wchar_t */

extern int iswctype(wchar_t c, unsigned short mask);

int uisspace(wchar_t c)
{
    return iswctype(c, 8u);
}
