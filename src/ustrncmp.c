#include <wchar.h>

/* ustrncmp — compare at most `count` characters of two UTF-16 strings. Tail-jump thunk
 * (single `b wcsncmp`) forwarding to the CRT wide bounded compare. */
/* wcsncmp provided by CRT via <wchar.h>; local extern removed (C28251: the local redeclaration drops the header's annotations) */

int ustrncmp(const wchar_t *string1, const wchar_t *string2, size_t count)
{
    return wcsncmp(string1, string2, count);
}
