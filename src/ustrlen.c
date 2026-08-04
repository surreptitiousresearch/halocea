/* ustrlen @0x8376D500 — wide-string length. Thunk to the CRT wcslen. */

#include <stddef.h>   /* wchar_t */

extern unsigned int wcslen(const wchar_t *string);

unsigned int ustrlen(const wchar_t *string)
{
    return wcslen(string);
}
