/* hs_concatenate_expression @0x83778418 — walk a HaloScript expression subtree, marking each node persistent
 * (permanent bit, 1<<3) and converting source-buffer references into entries in the string-constant buffer. Recurses over
 * argument lists for group (non-primitive) nodes. After conversion, source_offset holds the string-constant
 * buffer offset rather than the original source offset. */

#include "headers/hs_syntax_node.h"
#include "headers/hs_syntax_node_flags.h"
#include "headers/hs_compile_globals.h"
#include "headers/hs_function_definition.h"
#include "headers/data_array.h"
#include "headers/hs_type.h"
#include "headers/blam_data_globals.h"

extern int hs_concatenate_string_constant(const char *string);

void hs_concatenate_expression(int expression_index)
{
    hs_syntax_node *node = &HS_SYNTAX_NODE(expression_index);
    node->flags |= (1u << _hs_syntax_node_permanent_bit);

    if ( (node->flags & (1u << _hs_syntax_node_primitive_bit)) != 0 )       /* primitive expression */
    {
        if ( node->type == hs_function_name )          /* function-name / string-literal slot */
        {
            if ( node->source_offset == -1 )
                node->source_offset = hs_concatenate_string_constant(hs_function_table[node->___u1.function_index]->name);
            else
                node->source_offset = hs_concatenate_string_constant(&hs_compile_globals.compiled_source[node->source_offset]);
        }
        else if ( (node->flags & (1u << _hs_syntax_node_variable_bit)) != 0 || node->type >= hs_type_string )
        {
            node->source_offset = hs_concatenate_string_constant(&hs_compile_globals.compiled_source[node->source_offset]);
        }
    }
    else                                 /* group node: recurse over its argument list */
    {
        for ( int i = node->data; i != -1; i = HS_SYNTAX_NODE(i).next_node_index )
            hs_concatenate_expression(i);
    }
}
