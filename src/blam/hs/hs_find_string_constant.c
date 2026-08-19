/* hs_find_string_constant @0x83775DA0 — searches the HS string-constant buffer for an existing copy of
 * `string`; returns its byte offset, or -1 if not found (including an empty/absent buffer).
 *
 * DEVIATION: the decompiler renders the comparison as an inlined byte-difference loop; it is a plain
 * strcmp against each null-terminated entry in the buffer. */

#include "headers/hs_compile_globals.h"
#include <string.h>

int hs_find_string_constant(const char *string)
{
    if ( hs_compile_globals.string_constant_buffer_offset <= 0 )
        return -1;

    int offset = 0;
    while ( strcmp(string, &hs_compile_globals.string_constant_buffer[offset]) != 0 )
    {
        char *next = &hs_compile_globals.string_constant_buffer[offset];
        while ( *next )
            ++next;
        offset = (int)((next + 1) - hs_compile_globals.string_constant_buffer);

        if ( offset >= hs_compile_globals.string_constant_buffer_offset )
            return -1;
    }

    return offset;
}
