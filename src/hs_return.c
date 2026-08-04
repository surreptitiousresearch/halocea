/* hs_return @0x8368E398 — return a value from the currently executing HaloScript function/script on the
 * given thread: cast the value to the call's return type and store it through the parent frame's
 * child_result pointer, then pop the frame (thread->stack = thread->stack->parent).
 *
 * The current frame's expression is the call node; bit 1 of its flags marks a scenario script call (whose
 * return_type comes from the hs_script entry, DB offset 34) vs. a built-in (return_type from
 * hs_function_table); the node's type field is the desired cast type. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/hs_thread.h"
#include "headers/hs_syntax_node.h"
#include "headers/hs_syntax_node_flags.h"
#include "headers/hs_function_definition.h"
#include "headers/hs_script.h"
#include "headers/scenario.h"
#include "headers/blam_data_globals.h"

extern int hs_cast(int thread_index, int16_t actual_type, int16_t desired_type, int value);

void hs_return(uint16_t thread_index, int value)
{
    hs_thread *thread = DATA_ARRAY_ELEMENT(hs_thread_data, hs_thread, thread_index);
    hs_syntax_node *syntax_node = &HS_SYNTAX_NODE(thread->stack->expression_index);

    __int16 return_type;
    if ( (syntax_node->flags & (1u << _hs_syntax_node_script_bit)) != 0 )
        return_type = ((hs_script *)global_scenario->hs_scripts.address)[syntax_node->___u1.script_index].return_type;
    else
        return_type = hs_function_table[syntax_node->___u1.function_index]->return_type;

    *thread->stack->parent->child_result = hs_cast(thread_index, return_type, syntax_node->type, value);
    thread->stack = thread->stack->parent;   /* pop the frame */
}
