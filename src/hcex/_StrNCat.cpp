/* _StrNCat @ 0x823CCE70 -- HCEX bridge string helper (only caller: hcex_inst_create @0x823E74B8,
 * building the "__h" template-name-fallback suffix). Appends strSrc to the end of strDest,
 * truncating the appended portion (including the terminating NUL) to len bytes total -- i.e. a
 * bounds-checked strncat where `len` is the max number of bytes the *appended* text (result
 * NUL included) may occupy, not the full destination buffer size.
 *
 * NOTE: src/hcex/hcex_inst_create.c already documents its "__h" suffix append
 * (`strncat(hcexName, "__h", 0x40 - strlen(hcexName) - 1)`) as behaviorally equivalent to this
 * function and uses the CRT strncat() call there for readability; this file is the real,
 * separately linked _StrNCat symbol the binary actually calls at that site. */

#include <string.h>

void _StrNCat(char *strDest, const char *strSrc, unsigned int len)
{
    char *end = strDest;
    while (*end)
        ++end;

    unsigned int destLen = (unsigned int)(end - strDest);
    if (destLen < len)
    {
        unsigned int remaining = len - destLen;
        strncpy(end, strSrc, remaining);
        end[remaining - 1] = 0;
    }
}
