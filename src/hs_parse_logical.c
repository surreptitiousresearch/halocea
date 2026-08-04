/* hs_parse_logical @0x83779158 — parse a variadic logical operator (and / or). Every argument is forced to
 * boolean (type 5) and parsed; at least 2 arguments are required. */

#include <stdint.h>
#include "headers/hs_syntax_node.h"
#include "headers/hs_syntax_node_flags.h"
#include "headers/hs_compile_globals.h"
#include "headers/hs_function_definition.h"
#include "headers/data_array.h"
#include "headers/hs_type.h"
#include "headers/blam_data_globals.h"

extern uint8_t hs_parse_primitive(int expression_index);
extern uint8_t hs_parse_nonprimitive(int expression_index);
extern int sprintf_0(char *string, const char *format, ...);

int hs_parse_logical(int16_t function_index, int expression_index)
{
    hs_syntax_node *group = &HS_SYNTAX_NODE(expression_index);
    int result = 1;
    __int16 argument_count = 0;

    int argument = HS_SYNTAX_NODE(group->data).next_node_index;
    while ( argument != -1 )
    {
        result = 1;
        hs_syntax_node *arg_node = &HS_SYNTAX_NODE(argument);
        int next = arg_node->next_node_index;
        if ( !arg_node->type )
        {
            arg_node->type = hs_type_boolean;
            if ( (arg_node->flags & (1u << _hs_syntax_node_primitive_bit)) != 0 )
            {
                arg_node->___u1.constant_type = hs_type_boolean;
                result = hs_parse_primitive(argument);
            }
            else
            {
                result = hs_parse_nonprimitive(argument);
            }
        }
        ++argument_count;
        argument = next;
        if ( !(unsigned char)result )
            break;
    }

    if ( (unsigned char)result && argument_count < 2 )
    {
        sprintf_0(hs_compile_globals.error_buffer, "the %s call requires at least 2 arguments.",
                  hs_function_table[function_index]->name);
        hs_compile_globals.__noop = hs_compile_globals.error_buffer;
        hs_compile_globals.error_offset = group->source_offset;
        return 0;
    }
    return result;
}
