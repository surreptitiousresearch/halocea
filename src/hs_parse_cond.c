/* hs_parse_cond @0x83778E70 — parse (cond (<cond1> <expr1>) (<cond2> <expr2>) ...). The clause list is
 * rewritten (by hs_parse_cond_recursive) into a nested-if expression tree; the cond group node is then
 * overwritten in place by that tree's root node (preserving its own identifier and sibling link) and reparsed
 * against the original expected type. */

#include <stdint.h>
#include "headers/hs_syntax_node.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern int hs_parse_cond_recursive(int root_expression_index, int expression_index);
extern uint8_t hs_parse(int expression_index, int16_t expected_type);

int hs_parse_cond(int16_t function_index, int expression_index)
{
    hs_syntax_node *group = &HS_SYNTAX_NODE(expression_index);

    int first_clause = HS_SYNTAX_NODE(group->data).next_node_index;
    int built = hs_parse_cond_recursive(expression_index, first_clause);
    if ( built == -1 )
        return 0;

    int16_t saved_identifier = group->identifier;
    int saved_next = group->next_node_index;
    int16_t saved_type = group->type;

    HS_SYNTAX_NODE(built).next_node_index = saved_next;
    HS_SYNTAX_NODE(expression_index) = HS_SYNTAX_NODE(built);   /* overwrite cond group with built if-tree root */
    group->identifier = saved_identifier;

    return hs_parse(expression_index, saved_type);
}
