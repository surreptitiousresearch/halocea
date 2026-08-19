/* ustrcasecmp @0x8376D668 */
#include <wchar.h>

/* ustrcasecmp — case-insensitive compare of two UTF-16 strings. Implemented as a tail-jump thunk
 * (single `b _wcsicmp`) forwarding directly to the CRT wide, case-insensitive compare. */
/* _wcsicmp provided by CRT via <wchar.h>; local extern removed (C28251: the local redeclaration drops the header's annotations) */

int ustrcasecmp(const wchar_t *string1, const wchar_t *string2)
{
    return _wcsicmp(string1, string2);
}
