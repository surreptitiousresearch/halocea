#include <wchar.h>

/* ustrncat — append at most `count` characters of `source` onto `dest`. Tail-jump thunk
 * (single `b wcsncat`) forwarding to the CRT wide concatenate.
 * Attestation: 0/1 callers consume r3 — declared return is void. */

void ustrncat(wchar_t *dest, const wchar_t *source, unsigned int count)
{
    wcsncat(dest, source, count);
}
