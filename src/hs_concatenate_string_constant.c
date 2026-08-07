/* hs_concatenate_string_constant @0x83776EF8 — append `string` to the compiler's string-constant buffer,
 * de-duplicating against strings already present. Returns the buffer offset of the (existing or newly added)
 * copy, or -1 if it doesn't fit. */

#include <stdint.h>
#include <string.h>
#include "headers/hs_compile_globals.h"


extern int hs_find_string_constant(const char *string);
int hs_concatenate_string_constant(const char *string)
{
    /* DEVIATION: collapsed inlined body of hs_find_string_constant@0x83775DA0 (verbatim byte-diff search loop, zero xrefs, confirmed field-for-field + mnemonic-for-mnemonic at disasm grade) into a direct call. */
    /* DEVIATION: collapsed inlined body of hs_find_string_constant@0x83775DA0 (byte-diff search loop, confirmed instruction-for-instruction at disasm grade -- zero xrefs, fully inlined here) into a direct call; donor's -1 return corresponds to falling through to the append path below. */
    int found_offset = hs_find_string_constant(string);
    if ( found_offset != -1 )
        return found_offset;

    /* not found — append it */
    const char *scan = string;
    while ( *scan++ )
        ;
    int length = (int16_t)((uint16_t)(scan - string));
    if ( length < hs_compile_globals.string_constant_buffer_size )
    {
        int offset = hs_compile_globals.string_constant_buffer_offset;
        memcpy(&hs_compile_globals.string_constant_buffer[offset], string, length);
        hs_compile_globals.string_constant_buffer_offset += length;
        hs_compile_globals.string_constant_buffer_size -= length;
        return offset;
    }
    return -1;
}
