/* ustrstr @0x8376D580 */
#include <wchar.h>

/* ustrstr — locate the first occurrence of substring `needle` in `haystack`. Tail-jump thunk
 * (single `b wcsstr`) forwarding to the CRT wide substring search. */
/* wcsstr provided by CRT via <wchar.h>; local extern removed (C28251: the local redeclaration drops the header's annotations) */

wchar_t *ustrstr(const wchar_t *haystack, const wchar_t *needle)
{
    return wcsstr(haystack, needle);
}
