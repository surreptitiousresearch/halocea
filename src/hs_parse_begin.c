/* hs_parse_begin @0x83778A58 — parse the (begin ...) / (begin_random ...) statement blocks. The argument list
 * starts at the sibling of the group's function-name node (node.data -> name node -> next_node_index). For
 * begin (function_index 0) every non-final argument is forced to <void> (type 4) and the final argument carries
 * the block's return type, which is propagated back to the group. For begin_random (function_index 1) every
 * argument takes the group type and the argument count is capped at 32. An empty block is an error.
 *
 * function_index 0 == begin, 1 == begin_random (the first two entries of hs_function_table). */

#include <stdint.h>
#include "headers/hs_syntax_node.h"
#include "headers/hs_syntax_node_flags.h"
#include "headers/hs_compile_globals.h"
#include "headers/hs_function_definition.h"
#include "headers/data_array.h"
#include "headers/hs_function_index.h"
#include "headers/hs_type.h"
#include "headers/blam_data_globals.h"

extern uint8_t hs_parse_primitive(int expression_index);
extern uint8_t hs_parse_nonprimitive(int expression_index);
extern int sprintf_0(char *string, const char *format, ...);

int hs_parse_begin(__int16 function_index, int expression_index)
{
    hs_syntax_node *group = &HS_SYNTAX_NODE(expression_index);
    int result = 1;
    __int16 argument_count = 0;

    /* first real argument = sibling of the function-name node */
    int argument = HS_SYNTAX_NODE(group->data).next_node_index;
    while ( argument != -1 )
    {
        hs_syntax_node *arg_node = &HS_SYNTAX_NODE(argument);
        int next = arg_node->next_node_index;

        if ( function_index )
        {
            result = 1;
            __int16 group_type = group->type;
            if ( !arg_node->type )
            {
                arg_node->type = group_type;
                if ( (arg_node->flags & (1u << _hs_syntax_node_primitive_bit)) != 0 )
                {
                    arg_node->___u1.constant_type = group_type;
                    result = hs_parse_primitive(argument);
                }
                else
                {
                    result = hs_parse_nonprimitive(argument);
                }
            }
        }
        else
        {
            __int16 wanted = (next == -1) ? group->type : hs_type_void;
            result = 1;
            if ( !arg_node->type )
            {
                arg_node->type = wanted;
                if ( (arg_node->flags & (1u << _hs_syntax_node_primitive_bit)) != 0 )
                {
                    arg_node->___u1.constant_type = wanted;
                    result = hs_parse_primitive(argument);
                }
                else
                {
                    result = hs_parse_nonprimitive(argument);
                }
            }
            if ( next != -1 )
                goto advance;
        }

        if ( !group->type && (unsigned char)result )
            group->type = arg_node->type;
advance:
        argument = next;
        ++argument_count;
        if ( !(unsigned char)result )
            break;
    }

    if ( (unsigned char)result )
    {
        if ( argument_count >= 1 )
        {
            if ( argument_count > 32 && function_index == _hs_function_begin_random )
            {
                hs_compile_globals.__noop =
                    "begin_random can take a maximum of 32 arguments (matt can increase this.)";
                hs_compile_globals.error_offset = group->source_offset;
                return 0;
            }
        }
        else
        {
            sprintf_0(hs_compile_globals.error_buffer,
                      "a statement block must contain at least one argument.",
                      hs_function_table[function_index]->name);
            hs_compile_globals.__noop = hs_compile_globals.error_buffer;
            hs_compile_globals.error_offset = group->source_offset;
            return 0;
        }
    }
    return result;
}
