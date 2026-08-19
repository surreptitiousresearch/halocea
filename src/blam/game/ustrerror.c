/* ustrerror @0x8376DA40 — UTF-16 strerror: format the CRT strerror(err) narrow string into the
 * shared error_string buffer via usnprintf and return it. */

#include <wchar.h>
#include <string.h>

extern wchar_t error_string[256];                                                  /* @0x84469590 */
extern void usnprintf(wchar_t *string, unsigned int size, const wchar_t *format, ...);

wchar_t * ustrerror(int err)
{
    error_string[0] = 0;
    usnprintf(error_string, 256, L"%hs", strerror(err));
    return error_string;
}
