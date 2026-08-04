/* hcex_get_parent_script @0x8368D100 — hcex bridge (SSL script diagnostics): name of the scenario script
 * the currently executing HaloScript thread (hcex_cur_hs_thread) belongs to. Walks the thread's stack
 * frames upward (frame+0 = caller link, frame+4 = syntax node index; 20-byte syntax nodes): the first
 * frame whose syntax node has flag bit 1 (a script call) names the script (its script index in the node's
 * word +2; 92-byte hs_scripts elements start with the name). If no frame is a script call: internal
 * threads (thread type byte +2 nonzero) report "null", otherwise the thread's own script (dword +4). */

#include "../headers/data_array.h"
#include "../headers/hs_thread.h"
#include "../headers/hs_syntax_node.h"
#include "../headers/hs_syntax_node_flags.h"
#include "../headers/hs_script.h"
#include "../headers/scenario.h"

extern data_array *hs_thread_data;
extern data_array *hs_syntax_data;
extern scenario *global_scenario;
extern int hcex_cur_hs_thread;

extern "C" const char *hcex_get_parent_script(void)
{
    hs_thread *thread = DATA_ARRAY_ELEMENT(hs_thread_data, hs_thread, hcex_cur_hs_thread);

    for ( hs_stack_frame *frame = thread->stack->parent; frame; )
    {
        hs_stack_frame *caller_frame = frame->parent;
        if ( !caller_frame )
            break;
        hs_syntax_node *syntax_node = &HS_SYNTAX_NODE(frame->expression_index);
        if ( (syntax_node->flags & (1u << _hs_syntax_node_script_bit)) != 0 )
            return ((hs_script *)global_scenario->hs_scripts.address)[syntax_node->___u1.script_index].name;
        frame = caller_frame;
    }

    if ( thread->type )
        return "null";
    return ((hs_script *)global_scenario->hs_scripts.address)[thread->script_index].name;
}
