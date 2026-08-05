/* hs_evaluate_if @0x8368F050 — runtime of (if <cond> <then> [<else>]). Three 4-byte slots are pushed: the
 * condition result, the chosen branch expression, and the branch result. On the initialize pass the condition
 * is evaluated. On the next pass the true/false branch is selected and evaluated (an absent else with a false
 * condition returns 0). On the final pass the branch result is returned.
 *
 * thread+16 = active stack frame; frame+4 = current expression index; frame+12 = frame size (int16). */

#include <stdint.h>
#include "headers/hs_syntax_node.h"
#include "headers/hs_thread.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void hs_evaluate(int thread_index, int expression_index, int *destination);
extern void hs_return(uint16_t thread_index, int value);

void hs_evaluate_if(int16_t function_index, int thread_index, uint8_t initialize)
{
    hs_thread *thread = (hs_thread *)hs_thread_data->data + (uint16_t)thread_index;

    /* inlined hs_thread_stack_allocate x3: reserve slots at the frame data top, 4-aligned up.
     * The compiled `if (slot - 1 > top) --slot` correction can never fire (align-up advances at
     * most 3 bytes); kept verbatim for fidelity. */
    hs_stack_frame *frame = thread->stack;
    unsigned char *stack_top = &frame->data[frame->size];
    int *condition_result = (int *)(((unsigned int)stack_top + 3) & ~0x3u) /* align up to 4 */;
    if ( (unsigned int)(condition_result - 1) > (unsigned int)stack_top )   /* dead branch (shipped) */
        --condition_result;
    frame->size = (int16_t)((unsigned char *)condition_result - frame->data + 4);

    frame = thread->stack;
    stack_top = &frame->data[frame->size];
    int *chosen_branch = (int *)(((unsigned int)stack_top + 3) & ~0x3u) /* align up to 4 */;
    if ( (unsigned int)(chosen_branch - 1) > (unsigned int)stack_top )   /* dead branch (shipped) */
        --chosen_branch;
    frame->size = (int16_t)((unsigned char *)chosen_branch - frame->data + 4);

    frame = thread->stack;
    stack_top = &frame->data[frame->size];
    int *branch_result = (int *)(((unsigned int)stack_top + 3) & ~0x3u) /* align up to 4 */;
    if ( (unsigned int)(branch_result - 1) > (unsigned int)stack_top )   /* dead branch (shipped) */
        --branch_result;
    frame->size = (int16_t)((unsigned char *)branch_result - frame->data + 4);

    if ( initialize )
    {
        *condition_result = 0;
        *chosen_branch = -1;
        int if_expression = frame->expression_index;
        int name_node = HS_SYNTAX_NODE(if_expression).data;
        int condition = HS_SYNTAX_NODE(name_node).next_node_index;
        hs_evaluate(thread_index, condition, condition_result);
        return;
    }

    int result_value;
    if ( *chosen_branch == -1 )
    {
        int if_expression = frame->expression_index;
        int name_node = HS_SYNTAX_NODE(if_expression).data;
        int condition = HS_SYNTAX_NODE(name_node).next_node_index;

        if ( *(char *)condition_result != 0 )
        {
            *chosen_branch = HS_SYNTAX_NODE(condition).next_node_index;          /* then */
            hs_evaluate(thread_index, *chosen_branch, branch_result);
            return;
        }
        int then_branch = HS_SYNTAX_NODE(condition).next_node_index;
        *chosen_branch = HS_SYNTAX_NODE(then_branch).next_node_index;            /* else */
        if ( *chosen_branch != -1 )
        {
            hs_evaluate(thread_index, *chosen_branch, branch_result);
            return;
        }
        result_value = 0;
    }
    else
    {
        result_value = *branch_result;
    }
    hs_return(thread_index, result_value);
}
