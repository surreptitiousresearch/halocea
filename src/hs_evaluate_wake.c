/* hs_evaluate_wake @0x8368E4E8 — runtime of (wake <script>). Reads the script index from the call's argument
 * node, finds the sleeping thread running that script, and wakes it. Returns 0.
 *
 * The current expression index is held at frame+4 of the thread's active stack frame (thread+16). */

#include <stdint.h>
#include "headers/hs_syntax_node.h"
#include "headers/hs_thread.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern int hs_find_thread_by_script(int16_t script_index);
extern void hs_wake(uint16_t thread_index);
extern void hs_return(uint16_t thread_index, int value);

void hs_evaluate_wake(int16_t function_index, int thread_index, uint8_t initialize)
{
    hs_thread *thread = (hs_thread *)hs_thread_data->data + (unsigned __int16)thread_index;
    int call_expression = thread->stack->expression_index;

    int name_node = HS_SYNTAX_NODE(call_expression).data;
    int script_argument = HS_SYNTAX_NODE(name_node).next_node_index;
    __int16 script_index = (__int16)HS_SYNTAX_NODE(script_argument).data;

    int thread_by_script = hs_find_thread_by_script(script_index);
    if ( thread_by_script != -1 )
        hs_wake(thread_by_script);
    hs_return(thread_index, 0);
}
