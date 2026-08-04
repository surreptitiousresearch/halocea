/* ascii_to_wide @ 0x8376D9C8 — widen a NUL-terminated ASCII string into a UTF-16 buffer
 * (zero-extending each byte). Returns the output buffer, or NULL if it would overflow
 * unicode_length_bytes. Copies back-to-front so in-place use is safe.
 * Attestation: return path is `mr r3,r4` (unicode arg passthrough) or `li r3,0`, and callers
 * consume it as a pointer (NULL check @0x83782958, pointer subtraction @0x837668F0). */

#include <stddef.h>   /* wchar_t */

wchar_t *ascii_to_wide(const char *ascii, wchar_t *unicode, unsigned int unicode_length_bytes)
{
    const char *end = ascii;
    int len;
    int i;

    while ( *end++ )
        ;
    len = (int)(end - ascii - 1);

    if ( unicode_length_bytes < (unsigned int)(2 * len + 2) )
        return 0;

    unicode[len] = 0;
    for ( i = len - 1; i >= 0; --i )
        unicode[i] = (unsigned char)ascii[i];
    return unicode;
}
