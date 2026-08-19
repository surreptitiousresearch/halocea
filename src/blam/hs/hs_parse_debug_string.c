/* hs_parse_debug_string @0x83779B70 — parse the argument list of a debug print-style call. Every argument is
 * forced to string (type 9) and parsed; any number (including none) is accepted. */

#include <stdint.h>
#include "headers/hs_syntax_node.h"
#include "headers/hs_syntax_node_flags.h"
#include "headers/data_array.h"
#include "headers/hs_type.h"
#include "headers/blam_data_globals.h"

extern uint8_t hs_parse_primitive(int expression_index);
extern uint8_t hs_parse_nonprimitive(int expression_index);

int hs_parse_debug_string(int16_t function_index, int expression_index)
{
    int result = 1;
    int argument = HS_SYNTAX_NODE(HS_SYNTAX_NODE(expression_index).data).next_node_index;

    while ( argument != -1 )
    {
        result = 1;
        hs_syntax_node *arg_node = &HS_SYNTAX_NODE(argument);
        int next = arg_node->next_node_index;
        if ( !arg_node->type )
        {
            arg_node->type = hs_type_string;
            if ( (arg_node->flags & (1u << _hs_syntax_node_primitive_bit)) != 0 )
            {
                arg_node->___u1.constant_type = hs_type_string;
                result = hs_parse_primitive(argument);
            }
            else
            {
                result = hs_parse_nonprimitive(argument);
            }
        }
        argument = next;
        if ( !(unsigned char)result )
            break;
    }
    return result;
}
