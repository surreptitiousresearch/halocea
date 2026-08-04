#include <wchar.h>

/* ustrcasecmp — case-insensitive compare of two UTF-16 strings. Implemented as a tail-jump thunk
 * (single `b _wcsicmp`) forwarding directly to the CRT wide, case-insensitive compare. */
extern int _wcsicmp(const wchar_t *string1, const wchar_t *string2);

int ustrcasecmp(const wchar_t *string1, const wchar_t *string2)
{
    return _wcsicmp(string1, string2);
}
