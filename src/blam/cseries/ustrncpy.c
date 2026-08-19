/* ustrncpy @0x8376D560 — thunk: wide-string bounded copy (tail-jump `b wcsncpy`).
 * Attestation: 0/12 sampled callers (of 65) consume r3 — declared return is void. */

#include <wchar.h>

void ustrncpy(wchar_t *dest, const wchar_t *src, unsigned int count)
{
    wcsncpy(dest, src, count);
}
