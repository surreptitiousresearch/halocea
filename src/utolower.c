/* utolower @0x8376D6F8 — wide-char lowercase, thin wrapper over the CRT towlower. */

#include <wctype.h>

int utolower(wchar_t c)
{
    return towlower(c);
}
