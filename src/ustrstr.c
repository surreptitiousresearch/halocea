#include <wchar.h>

/* ustrstr — locate the first occurrence of substring `needle` in `haystack`. Tail-jump thunk
 * (single `b wcsstr`) forwarding to the CRT wide substring search. */
extern wchar_t *wcsstr(const wchar_t *haystack, const wchar_t *needle);

wchar_t *ustrstr(const wchar_t *haystack, const wchar_t *needle)
{
    return wcsstr(haystack, needle);
}
