/* hs_arguments_evaluate @0x8368E780 — drive evaluation of a function call's actual arguments one at a time on
 * a HaloScript thread. On the `initialize` call it bump-allocates three slots in the thread's stack frame (an
 * argument-value array, a 2-byte argument cursor, and a 4-byte "current argument node" pointer) and seeds the
 * cursor to the call's first argument node. On every call it evaluates the current argument (if its type
 * matches the next formal parameter), advances the cursor, and returns null to signal "more to do"; once the
 * arguments are exhausted (or a type mismatch ends them) it returns the argument-value array.
 *
 * Threads are 536-byte data_array data; the stack-frame pointer is at thread+16 and that frame keeps its
 * current fill size as an int16 at frame+12. The allocation idiom (round up, step back if it overshot the
 * available space) is reproduced faithfully from the compiled code. */

#include <stdint.h>
#include "headers/hs_syntax_node.h"
#include "headers/hs_thread.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void hs_evaluate(int thread_index, int expression_index, int *destination);

int * hs_arguments_evaluate(int thread_index, int16_t formal_parameter_count, const int16_t *formal_parameters, uint8_t initialize)
{
    hs_thread *thread = (hs_thread *)hs_thread_data->data + (uint16_t)thread_index;

    /* slot 1: argument-value array (4-byte aligned, formal_parameter_count entries).
     * inlined hs_thread_stack_allocate: reserve at the frame data top, aligned up. The compiled
     * `if (slot - elem > top) slot -= elem` correction can never fire (align-up advances at most
     * 3 bytes); kept verbatim for fidelity. */
    hs_stack_frame *frame = thread->stack;
    unsigned char *stack_top = &frame->data[frame->size];
    int *values = (int *)(((uintptr_t)stack_top + 3) & ~(uintptr_t)3) /* align up to 4 */;
    if ( (uintptr_t)(values - 1) > (uintptr_t)stack_top )   /* dead branch (shipped) */
        --values;
    frame->size = (int16_t)((unsigned char *)values - frame->data + 4 * formal_parameter_count);

    /* slot 2: argument cursor (2-byte aligned, one int16) */
    frame = thread->stack;
    stack_top = &frame->data[frame->size];
    int16_t *argument_cursor = (int16_t *)(((uintptr_t)stack_top + 1) & ~(uintptr_t)1) /* align up to 2 */;
    if ( (uintptr_t)(argument_cursor - 1) > (uintptr_t)stack_top )   /* dead branch (shipped) */
        --argument_cursor;
    frame->size = (int16_t)((unsigned char *)argument_cursor - frame->data + 2);

    /* slot 3: current-argument node index (4-byte aligned, one int) */
    frame = thread->stack;
    stack_top = &frame->data[frame->size];
    int *current_argument = (int *)(((uintptr_t)stack_top + 3) & ~(uintptr_t)3) /* align up to 4 */;
    if ( (uintptr_t)(current_argument - 1) > (uintptr_t)stack_top )   /* dead branch (shipped) */
        --current_argument;
    frame->size = (int16_t)((unsigned char *)current_argument - frame->data + 4);

    if ( initialize )
    {
        *argument_cursor = 0;
        /* seed to the first argument: node after the call node's first child */
        int call_node = (thread->stack)->expression_index;
        *current_argument = HS_SYNTAX_NODE(HS_SYNTAX_NODE(call_node).data).next_node_index;
    }

    int index = *argument_cursor;
    if ( index < formal_parameter_count
      && HS_SYNTAX_NODE(*current_argument).type == (uint16_t)formal_parameters[index] )
    {
        hs_evaluate(thread_index, *current_argument, &values[index]);
        *current_argument = HS_SYNTAX_NODE(*current_argument).next_node_index;
        ++*argument_cursor;
        return nullptr;
    }
    return values;
}
