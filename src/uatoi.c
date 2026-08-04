#include <wchar.h>

/* uatoi — parse a UTF-16 string as a base-10 integer. Tail-jump thunk (single `b _wtoi`)
 * forwarding to the CRT wide ASCII-to-integer routine. */
extern int _wtoi(const wchar_t *string);

int uatoi(const wchar_t *string)
{
    return _wtoi(string);
}
