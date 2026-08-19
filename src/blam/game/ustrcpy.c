/* ustrcpy @0x8376D4A8 — wide-string copy (thin wrapper over wcscpy). The leading wcslen call computes the source
 * length but discards it (a debug/length-check remnant in the original).
 * Attestation: r3 at blr is wcscpy residue and 0/10 sampled callers consume it — declared return is void. */

#include <wchar.h>

void ustrcpy(wchar_t *dest, const wchar_t *src)
{
    wcslen(src);   /* result discarded in the binary too */
    wcscpy(dest, src);
}
