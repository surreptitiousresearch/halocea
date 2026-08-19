/* hs_parse_real @0x83776248 — parse a real (float) atom. Validates that the token is digits with an optional
 * leading '-' and at most one '.', then stores atof(token) into the node's data field (as float). A malformed
 * token reports an error but the atof result is still stored.
 *
 * Deviation: the decompiler reads the digit class via __pctype_func()[c]&4 (the CRT _DIGIT mask); reproduced
 * with isdigit. */

#include "headers/hs_syntax_node.h"
#include "headers/hs_compile_globals.h"
#include "headers/data_array.h"
#include <ctype.h>
#include "headers/blam_data_globals.h"

extern double atof(const char *s);

int hs_parse_real(int expression_index)
{
    hs_syntax_node *node = &HS_SYNTAX_NODE(expression_index);
    int valid = 1;
    char seen_decimal = 0;
    const char *cursor = &hs_compile_globals.compiled_source[node->source_offset];

    if ( *cursor == '-' )
        ++cursor;
    for ( ; *cursor; ++cursor )
    {
        int c = (unsigned char)*cursor;
        if ( !isdigit(c) )
        {
            if ( seen_decimal || c != '.' )
            {
                valid = 0;
                hs_compile_globals.__noop = "this is not a valid real number.";
                hs_compile_globals.error_offset = node->source_offset;
                break;
            }
            seen_decimal = 1;
        }
    }

    *(float *)&node->data = (float)atof(&hs_compile_globals.compiled_source[node->source_offset]);
    return valid;
}
