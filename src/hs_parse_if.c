/* hs_parse_if @0x83778C98 — parse (if <condition> <then> [<else>]). Requires two or three arguments; the
 * condition is parsed as boolean (type 5) and the then/else branches as the group's expected type. When the
 * group type is not yet known it is inferred from whichever branch parses, and the other branch is then
 * re-parsed against it.
 *
 * function_index is unused (if has a single table entry). */

#include <stdint.h>
#include "headers/hs_syntax_node.h"
#include "headers/hs_compile_globals.h"
#include "headers/data_array.h"
#include "headers/hs_type.h"
#include "headers/blam_data_globals.h"

extern uint8_t hs_parse(int expression_index, int16_t expected_type);

int hs_parse_if(int16_t function_index, int expression_index)
{
    hs_syntax_node *group = &HS_SYNTAX_NODE(expression_index);

    int condition = HS_SYNTAX_NODE(group->data).next_node_index;
    int then_branch = (condition == -1) ? -1 : HS_SYNTAX_NODE(condition).next_node_index;
    int else_branch = (then_branch == -1) ? -1 : HS_SYNTAX_NODE(then_branch).next_node_index;

    if ( condition == -1
      || then_branch == -1
      || (else_branch != -1 && HS_SYNTAX_NODE(else_branch).next_node_index != -1) )
    {
        hs_compile_globals.__noop = "i expected (if <condition> <then> [<else>]).";
        hs_compile_globals.error_offset = group->source_offset;
        return 0;
    }

    if ( !hs_parse(condition, hs_type_boolean) )
        return 0;

    if ( !hs_parse(then_branch, group->type) )
    {
        if ( !hs_compile_globals.__noop && !group->type && else_branch != -1
          && hs_parse(else_branch, group->type) )
        {
            int16_t inferred = HS_SYNTAX_NODE(else_branch).type;
            group->type = inferred;
            return hs_parse(then_branch, inferred);
        }
        return 0;
    }

    if ( !group->type )
        group->type = HS_SYNTAX_NODE(then_branch).type;
    if ( else_branch == -1 )
        return 1;
    return hs_parse(else_branch, group->type) ? 1 : 0;
}
