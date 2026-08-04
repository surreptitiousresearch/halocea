/* hs_parse_call_predicate @0x83775FB8 — resolve the head atom of a "(...)" group to a function or script index
 * and stash it on the call node. If the head was already resolved (type 2) it just copies the index across;
 * otherwise it looks the name up as a function, then (if that fails) as a script — marking the call node with
 * flag 2 when it resolves to a script. */

#include <stdint.h>
#include "headers/hs_syntax_node.h"
#include "headers/hs_syntax_node_flags.h"
#include "headers/hs_compile_globals.h"
#include "headers/data_array.h"
#include "headers/hs_type.h"
#include "headers/blam_data_globals.h"

extern int16_t hs_find_function_by_name(const char *name);
extern int16_t hs_find_script_by_name(const char *name);

void hs_parse_call_predicate(int expression_index)
{
    hs_syntax_node *node = &HS_SYNTAX_NODE(expression_index);
    hs_syntax_node *head = &HS_SYNTAX_NODE(node->data);

    if ( head->type == hs_function_name )
    {
        node->___u1.function_index = head->___u1.function_index;
        return;
    }

    const char *name = &hs_compile_globals.compiled_source[head->source_offset];
    node->___u1.function_index = hs_find_function_by_name(name);
    head->type = hs_function_name;
    __int16 resolved = node->___u1.function_index;
    if ( (unsigned __int16)resolved == 0xFFFF )
    {
        resolved = hs_find_script_by_name(name);
        node->___u1.function_index = resolved;
        if ( resolved != -1 )
            node->flags |= (1u << _hs_syntax_node_script_bit);
    }
    head->___u1.function_index = resolved;
}
