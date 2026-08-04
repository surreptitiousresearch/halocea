/* hs_get_parameter_indices @0x83775B38 — collect the expression indices of a call's arguments (the siblings
 * following the function-name node) into result_indices, requiring exactly `count` of them. Reports
 * "the <name> call requires <count> arguments." on any mismatch (too few or too many). */

#include <stdint.h>
#include "headers/hs_syntax_node.h"
#include "headers/hs_compile_globals.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern int sprintf_0(char *string, const char *format, ...);

uint8_t hs_get_parameter_indices(const char *function_name, int16_t count, int *result_indices, int expression_index)
{
    hs_syntax_node *group = &HS_SYNTAX_NODE(expression_index);
    __int16 collected = 0;

    int argument = HS_SYNTAX_NODE(group->data).next_node_index;
    for ( ; argument != -1; argument = HS_SYNTAX_NODE(argument).next_node_index )
    {
        if ( collected >= count )
            break;
        result_indices[collected++] = argument;
    }

    if ( collected != count || argument != -1 )
    {
        sprintf_0(hs_compile_globals.error_buffer, "the %s call requires %d arguments.", function_name, count);
        hs_compile_globals.__noop = hs_compile_globals.error_buffer;
        hs_compile_globals.error_offset = group->source_offset;
        return 0;
    }
    return 1;
}
