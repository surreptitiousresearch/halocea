/* expression_get_function_name @0x8368CC08 — diagnostic name lookup for a HaloScript expression: if the
 * expression's own syntax node is a script-call node (_hs_syntax_node_script_bit), returns that script's
 * name directly (hs_script.name). Otherwise walks the thread's current evaluation frame
 * (hs_thread.stack, an hs_stack_frame): while the innermost frame is evaluating our expression
 * (frame->expression_index), it follows the frame's first data slot (frame->data[0]) as the next
 * expression to inspect, returning a script's name if that node is a script call, or "(end of script)"
 * when the chain reaches -1. If it breaks out because a frame doesn't match, returns the function name for
 * the value last read from the ORIGINAL expression's syntax node (___u1.function_index) — 0 there is what
 * drops into the walk in the first place, so this is effectively "the calling function's name" once the
 * frame walk stops advancing. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/scenario.h"
#include "headers/hs_function_definition.h"
#include "headers/hs_syntax_node.h"
#include "headers/hs_syntax_node_flags.h"
#include "headers/hs_thread.h"
#include "headers/hs_stack_frame.h"
#include "headers/hs_script.h"
#include "headers/blam_data_globals.h"


char * expression_get_function_name(uint16_t thread_index, int expression_index)
{
    hs_syntax_node *syntax_data = (hs_syntax_node *)hs_syntax_data->data;
    hs_syntax_node *syntax_node = &syntax_data[(unsigned __int16)expression_index];
    hs_thread *thread = DATA_ARRAY_ELEMENT(hs_thread_data, hs_thread, thread_index);
    hs_script *scripts = (hs_script *)global_scenario->hs_scripts.address;

    if ( (syntax_node->flags & (1u << _hs_syntax_node_script_bit)) != 0 )
        return scripts[syntax_node->___u1.script_index].name;

    __int16 function_or_script_id;
    while ( 1 )
    {
        function_or_script_id = syntax_node->___u1.function_index;
        if ( function_or_script_id )
            break;

        hs_stack_frame *frame = thread->stack;
        if ( expression_index != frame->expression_index )
            break;

        /* this diagnostic follows the innermost frame's first data slot as the next expression to inspect */
        int next_expression_index = *(int *)frame->data;
        if ( next_expression_index == -1 )
            return "(end of script)";

        expression_index = next_expression_index;
        thread = DATA_ARRAY_ELEMENT(hs_thread_data, hs_thread, thread_index);
        syntax_node = &syntax_data[(unsigned __int16)next_expression_index];
        if ( (syntax_node->flags & (1u << _hs_syntax_node_script_bit)) != 0 )
            return scripts[syntax_node->___u1.script_index].name;
    }

    return hs_function_table[function_or_script_id]->name;
}
