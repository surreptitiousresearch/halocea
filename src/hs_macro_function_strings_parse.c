/* hs_macro_function_strings_parse @0x83778978 — force every argument of an hs macro-function call
 * expression to parse as type string (9): the call expression's data field holds the function-name node
 * index whose next_node_index heads the parameter chain; for each node whose value type is still unset,
 * set it to string and parse it (primitive when flags bit 0 is set, marking constant_type as string too).
 * Stops on the first parse failure. function_index is unused (macro id, for symmetry with the other
 * hs_*_parse handlers). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/hs_syntax_node.h"
#include "headers/hs_syntax_node_flags.h"
#include "headers/hs_type.h"
#include "headers/blam_data_globals.h"


extern uint8_t hs_parse_primitive(int expression_index);
extern uint8_t hs_parse_nonprimitive(int expression_index);

int hs_macro_function_strings_parse(__int16 function_index, int expression_index)
{
    uint8_t ok = 1;

    /* first parameter: follow the call expression's data (function-name node) to its successor */
    int parameter_index = HS_SYNTAX_NODE(HS_SYNTAX_NODE(expression_index).data).next_node_index;
    do
    {
        if ( parameter_index == -1 )
            break;
        ok = 1;
        hs_syntax_node *node = &HS_SYNTAX_NODE(parameter_index);
        if ( !node->type )
        {
            node->type = hs_type_string;   /* value type = string */
            if ( (node->flags & (1u << _hs_syntax_node_primitive_bit)) != 0 )
            {
                node->___u1.constant_type = hs_type_string;
                ok = hs_parse_primitive(parameter_index);
            }
            else
            {
                ok = hs_parse_nonprimitive(parameter_index);
            }
        }
        parameter_index = node->next_node_index;
    }
    while ( ok );

    return ok;
}
