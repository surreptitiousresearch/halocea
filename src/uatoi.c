#include <wchar.h>

/* uatoi — parse a UTF-16 string as a base-10 integer. Tail-jump thunk (single `b _wtoi`)
 * forwarding to the CRT wide ASCII-to-integer routine. */
/* _wtoi provided by CRT via <wchar.h>; local extern removed (C28251: the local redeclaration drops the header's annotations) */

int uatoi(const wchar_t *string)
{
    return _wtoi(string);
}
