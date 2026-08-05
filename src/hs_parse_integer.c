/* hs_parse_integer @0x83776330 — parse an integer atom (shared by the short [type 7] and long [type 8] slots).
 * Validates digits with an optional leading '-', then stores atoi(token). For a non-long slot the value must fit
 * in a signed short. A long slot stores the full dword in data; otherwise the short is stored in data (word).
 *
 * Deviation: digit class read via isdigit instead of the inlined __pctype_func()[c]&4. The short-range error
 * message reads "[-32767, 32768]" verbatim from the binary even though the implemented bounds are
 * (> 0x7FFF || < -32768). */

#include <stdint.h>
#include "headers/hs_syntax_node.h"
#include "headers/hs_compile_globals.h"
#include "headers/data_array.h"
#include <ctype.h>
#include "headers/hs_type.h"
#include "headers/blam_data_globals.h"

extern int atoi(const char *s);

int hs_parse_integer(int expression_index)
{
    hs_syntax_node *node = &HS_SYNTAX_NODE(expression_index);
    int valid = 1;
    const char *cursor = &hs_compile_globals.compiled_source[node->source_offset];

    if ( *cursor == '-' )
        ++cursor;
    if ( *cursor )
    {
        while ( isdigit((unsigned char)*cursor) )
        {
            if ( !*++cursor )
                goto parse;
        }
        valid = 0;
        hs_compile_globals.__noop = "this is not a valid integer.";
        hs_compile_globals.error_offset = node->source_offset;
    }
parse:
    {
        int value = atoi(&hs_compile_globals.compiled_source[node->source_offset]);
        if ( (unsigned char)valid && node->type != hs_type_long_integer && (value > 0x7FFF || value < -32768) )
        {
            valid = 0;
            hs_compile_globals.__noop = "shorts must be in the range [-32767, 32768].";
            hs_compile_globals.error_offset = node->source_offset;
        }
        if ( node->type == hs_type_long_integer )
            node->data = value;
        else
            *(int16_t *)&node->data = value;
        return valid;
    }
}
