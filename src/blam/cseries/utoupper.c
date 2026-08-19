/* utoupper @0x8376D6D0 — wide-char uppercase, thin wrapper over the CRT towupper. */

#include <wctype.h>

int utoupper(wchar_t c)
{
    return towupper(c);
}
