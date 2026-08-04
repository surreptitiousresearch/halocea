/* ustrcmp @0x8376D4F8 — thunk: wide-string compare (Unicode string compare). */

#include <wchar.h>

int ustrcmp(const wchar_t *string1, const wchar_t *string2)
{
    return wcscmp(string1, string2);
}
