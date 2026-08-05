/* hs_macro_function_parse @0x837787E0 — type/parse-check every argument of a HaloScript macro function
 * call. Walks the call expression's argument chain (the node pointed to by the call's own `data` field is
 * a head node; the real first argument is that head's `next_node_index`), assigning each unset argument
 * node's `type` from the function's formal parameter list, then parsing it as either a primitive constant
 * (formal parameter flag bit 0 set) or a nested expression, stopping early on the first parse failure or
 * once the formal parameter count is exhausted. If parsing completes and the argument count matches
 * exactly (arity == formal_parameter_count and the chain ends in -1), returns 1; if there are too many or
 * too few arguments, records a "requires exactly N arguments" compile error and returns 0. On any argument
 * parse failure, returns 0 without emitting a new error message (the failing parse already reported one).
 *
 * DEVIATION: the DB prototype confirms the return type is `unsigned __int8`, not the raw decompile's `int`.
 * `hs_syntax_data->data` is deliberately re-read via the HS_SYNTAX_NODE() macro after every
 * hs_parse_primitive/hs_parse_nonprimitive call rather than cached across it — those calls can grow the
 * syntax array and reallocate its backing storage, which the original code (and this reconstruction)
 * carefully accounts for by never holding a raw hs_syntax_node* across a parse call. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/hs_syntax_node.h"
#include "headers/hs_function_definition.h"
#include "headers/hs_compile_globals.h"
#include "headers/blam_data_globals.h"


extern uint8_t hs_parse_primitive(int expression_index);
extern uint8_t hs_parse_nonprimitive(int expression_index);
extern int sprintf_0(char *string, const char *format, ...);

uint8_t hs_macro_function_parse(int16_t function_index, int expression_index)
{
    const hs_function_definition *function = hs_function_table[function_index];
    int arg_index = HS_SYNTAX_NODE(HS_SYNTAX_NODE(expression_index).data).next_node_index;

    uint8_t success = 1;
    int16_t param_index = 0;

    while ( param_index < function->formal_parameter_count && arg_index != -1 )
    {
        int16_t formal_type = function->formal_parameters[param_index];
        uint8_t parsed = 1;

        if ( !HS_SYNTAX_NODE(arg_index).type )
        {
            HS_SYNTAX_NODE(arg_index).type = formal_type;

            if ( (HS_SYNTAX_NODE(arg_index).flags & 1) != 0 )
            {
                HS_SYNTAX_NODE(arg_index).___u1.function_index = formal_type;
                parsed = hs_parse_primitive(arg_index);
            }
            else
            {
                parsed = hs_parse_nonprimitive(arg_index);
            }
        }

        if ( !parsed )
        {
            success = 0;
            break;
        }

        arg_index = HS_SYNTAX_NODE(arg_index).next_node_index;
        ++param_index;
    }

    if ( !success )
        return success;

    int16_t formal_parameter_count = function->formal_parameter_count;
    if ( param_index == formal_parameter_count && arg_index == -1 )
        return success;

    sprintf_0(hs_compile_globals.error_buffer, "the \"%s\" call requires exactly %d arguments.", function->name,
            formal_parameter_count);
    hs_compile_globals.__noop = hs_compile_globals.error_buffer;
    hs_compile_globals.error_offset = HS_SYNTAX_NODE(expression_index).source_offset;
    return 0;
}
