/* hs_evaluate_begin @0x8368EBF0 — runtime of (begin ...). Two 4-byte slots are pushed on the thread frame: an
 * iterator holding the current child expression and a result accumulator. Each pass evaluates the current child
 * into the result slot and advances the iterator; when the children are exhausted the last result is returned.
 *
 * thread+16 = active stack frame; frame+4 = current expression index; frame+12 = frame size (int16). The raw
 * bump-allocation of each value slot is reproduced verbatim. */

#include <stdint.h>
#include "headers/hs_syntax_node.h"
#include "headers/hs_thread.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void hs_evaluate(int thread_index, int expression_index, int *destination);
extern void hs_return(int thread_index, int value);

void hs_evaluate_begin(int16_t function_index, int thread_index, uint8_t initialize)
{
    hs_thread *thread = (hs_thread *)hs_thread_data->data + (uint16_t)thread_index;

    /* inlined hs_thread_stack_allocate x2: reserve slots at the frame data top, 4-aligned up.
     * The compiled `if (slot - 1 > top) --slot` correction can never fire (align-up advances at
     * most 3 bytes); kept verbatim for fidelity. */
    hs_stack_frame *frame = thread->stack;
    unsigned char *stack_top = &frame->data[frame->size];
    int *iterator = (int *)(((uintptr_t)stack_top + 3) & ~(uintptr_t)0x3);
    if ( (uintptr_t)(iterator - 1) > (uintptr_t)stack_top )   /* dead branch (shipped) */
        --iterator;
    frame->size = (int16_t)((unsigned char *)iterator - frame->data + 4);

    frame = thread->stack;
    stack_top = &frame->data[frame->size];
    int *result = (int *)(((uintptr_t)stack_top + 3) & ~(uintptr_t)0x3);
    if ( (uintptr_t)(result - 1) > (uintptr_t)stack_top )   /* dead branch (shipped) */
        --result;
    frame->size = (int16_t)((unsigned char *)result - frame->data + 4);

    if ( initialize )
    {
        int begin_expression = frame->expression_index;
        int name_node = HS_SYNTAX_NODE(begin_expression).data;
        *iterator = HS_SYNTAX_NODE(name_node).next_node_index;
        *result = 0;
    }

    if ( *iterator == -1 )
    {
        hs_return(thread_index, *result);
    }
    else
    {
        hs_evaluate(thread_index, *iterator, result);
        *iterator = HS_SYNTAX_NODE(*iterator).next_node_index;
    }
}
