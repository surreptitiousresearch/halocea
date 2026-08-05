/* hs_parse_cond_recursive @0x83775860 — recursively rewrite a cond clause list into a nested-if syntax tree.
 * For clause (<condition> <result>) it synthesises (if <condition> <result> <rest>) where <rest> is the tree
 * built from the following clauses; the recursion bottoms out (expression_index == -1) in a typed no-op that
 * supplies cond's value type. Returns the new root node index, or -1 on error.
 *
 * Note: every node pointer is taken fresh through HS_SYNTAX_NODE after each datum_new, because datum_new may
 * grow (reallocate) the hs_syntax_data array — the decompiler reloads the base pointer for the same reason.
 *
 * SHIPPED BUG (verified in disasm at 0x83775964): the "needs a result" guard is DEAD. The binary computes
 * (next_node_index == 0) == -1 — cntlzw r6,r7 / extrwi r5,r6,1,26 yields r5 = (next == 0), then cmpwi r5,-1 /
 * beq, and a 0/1 boolean is never -1, so the branch is never taken. This is NOT a decompiler artifact (the
 * decompiler renders it faithfully); it is a genuine game bug: a cond clause with a condition but no result
 * expression is NOT rejected by the retail build. The intended predicate was next_node_index == -1. We
 * reproduce the binary's actual (always-false) guard below to stay faithful to shipped behavior. */

#include <stdint.h>
#include "headers/hs_syntax_node.h"
#include "headers/hs_syntax_node_flags.h"
#include "headers/hs_compile_globals.h"
#include "headers/data_array.h"
#include "headers/hs_type.h"
#include "headers/blam_data_globals.h"
#include "headers/hs_function_index.h"

extern int datum_new(data_array *data);

int hs_parse_cond_recursive(int root_expression_index, int expression_index)
{
    int new_index = datum_new(hs_syntax_data);
    if ( new_index == -1 )
    {
        hs_compile_globals.__noop = "i couldn't allocate a syntax node.";
        hs_compile_globals.error_offset = HS_SYNTAX_NODE(root_expression_index).source_offset;
        return -1;
    }

    int root_source_offset = HS_SYNTAX_NODE(root_expression_index).source_offset;
    HS_SYNTAX_NODE(new_index).next_node_index = -1;
    HS_SYNTAX_NODE(new_index).flags = 0;
    HS_SYNTAX_NODE(new_index).source_offset = root_source_offset;

    if ( expression_index == -1 )
    {
        int16_t root_type = HS_SYNTAX_NODE(root_expression_index).type;
        HS_SYNTAX_NODE(new_index).flags = (1u << _hs_syntax_node_primitive_bit);
        HS_SYNTAX_NODE(new_index).data = 0;
        HS_SYNTAX_NODE(new_index).___u1.constant_type = root_type;
        HS_SYNTAX_NODE(new_index).type = root_type;
        return new_index;
    }

    if ( (HS_SYNTAX_NODE(expression_index).flags & (1u << _hs_syntax_node_primitive_bit)) != 0 )
    {
        hs_compile_globals.__noop = "this argument to cond should be a condition/result pair";
        hs_compile_globals.error_offset = HS_SYNTAX_NODE(expression_index).source_offset;
        return -1;
    }

    int condition_index = HS_SYNTAX_NODE(expression_index).data;
    /* Faithful to the binary: always-false guard, see SHIPPED BUG note in the file header.
     * (Intended predicate was: HS_SYNTAX_NODE(condition_index).next_node_index == -1.) */
    if ( (HS_SYNTAX_NODE(condition_index).next_node_index == 0) == -1 )
    {
        hs_compile_globals.__noop = "this argument to cond needs a result.";
        hs_compile_globals.error_offset = HS_SYNTAX_NODE(condition_index).source_offset;
        return -1;
    }

    int if_then_index = datum_new(hs_syntax_data);
    int then_value_index = datum_new(hs_syntax_data);
    if ( if_then_index == -1 || then_value_index == -1 )
    {
        hs_compile_globals.__noop = "i couldn't allocate a syntax node.";
        hs_compile_globals.error_offset = HS_SYNTAX_NODE(root_expression_index).source_offset;
        return -1;
    }

    int rest = hs_parse_cond_recursive(root_expression_index, HS_SYNTAX_NODE(expression_index).next_node_index);
    HS_SYNTAX_NODE(if_then_index).next_node_index = rest;
    if ( rest == -1 )
        return -1;

    int result_expression = HS_SYNTAX_NODE(condition_index).next_node_index;

    HS_SYNTAX_NODE(new_index).data = expression_index;
    HS_SYNTAX_NODE(new_index).___u1.constant_type = hs_function_name;

    /* rewrite the clause group node into an (if ...) call (function-name slot, type 2) */
    HS_SYNTAX_NODE(expression_index).data = 0;
    HS_SYNTAX_NODE(expression_index).___u1.function_index = _hs_function_if;
    HS_SYNTAX_NODE(expression_index).flags = (1u << _hs_syntax_node_primitive_bit);
    HS_SYNTAX_NODE(expression_index).next_node_index = condition_index;
    HS_SYNTAX_NODE(expression_index).source_offset = -1;
    HS_SYNTAX_NODE(expression_index).type = hs_function_name;

    HS_SYNTAX_NODE(if_then_index).data = then_value_index;
    HS_SYNTAX_NODE(if_then_index).flags = 0;
    HS_SYNTAX_NODE(if_then_index).source_offset = HS_SYNTAX_NODE(new_index).source_offset;

    HS_SYNTAX_NODE(then_value_index).data = 0;
    HS_SYNTAX_NODE(then_value_index).___u1.script_index = 0;
    HS_SYNTAX_NODE(then_value_index).flags = (1u << _hs_syntax_node_primitive_bit);
    HS_SYNTAX_NODE(then_value_index).next_node_index = result_expression;
    HS_SYNTAX_NODE(then_value_index).source_offset = -1;
    HS_SYNTAX_NODE(then_value_index).type = hs_function_name;

    HS_SYNTAX_NODE(condition_index).next_node_index = if_then_index;
    return new_index;
}
