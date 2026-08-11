#include <stdint.h>
#include <wchar.h>
/* wide_to_ascii @0x8376D940 — convert a wide (UTF-16) string to ASCII, substituting a space for any code
 * unit that doesn't fit in a byte. Fails (returns null) if the source doesn't fit in the destination
 * (including the NUL). Returns the destination on success. */


char * wide_to_ascii(uint16_t *unicode, char *ascii, unsigned int ascii_length_bytes)
{
    unsigned int length = wcslen(unicode);
    if ( length > ascii_length_bytes - 1 )
        return 0;

    int out = 0;
    for ( unsigned int i = 0; i < length; ++i )
    {
        int code = unicode[i];
        if ( (code & 0xFFFFFF00) != 0 )
            ascii[out] = 32;
        else
            ascii[out] = code;
        ++out;
    }
    ascii[out] = 0;
    return ascii;
}
