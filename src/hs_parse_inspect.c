/* hs_parse_inspect @0x83779A18 — parse (inspect <expression>). The single argument is parsed untyped (type 0);
 * if it has no inspectable form a specific error is reported (unless one was already set). */

#include <stdint.h>
#include "headers/hs_syntax_node.h"
#include "headers/hs_compile_globals.h"
#include "headers/hs_function_definition.h"
#include "headers/data_array.h"
#include "headers/hs_type.h"
#include "headers/blam_data_globals.h"

extern uint8_t hs_get_parameter_indices(const char *function_name, int16_t count, int *result_indices, int expression_index);
extern uint8_t hs_parse(int expression_index, int16_t expected_type);

int hs_parse_inspect(int16_t function_index, int expression_index)
{
    int argument;
    if ( !hs_get_parameter_indices(hs_function_table[function_index]->name, 1, &argument, expression_index) )
        return 0;

    if ( hs_parse(argument, hs_unparsed) )
        return 1;

    if ( !hs_compile_globals.__noop )
    {
        hs_compile_globals.__noop = "this is not a global variable reference, function call, or script call.";
        hs_compile_globals.error_offset = HS_SYNTAX_NODE(argument).source_offset;
    }
    return 0;
}
