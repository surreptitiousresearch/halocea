/* hs_evaluate_logical @0x8368F428 — runtime of (and ...) / (or ...). Three frame slots are pushed: the child
 * iterator, a per-operand value slot, and a 1-byte boolean accumulator. and (function_index 5) seeds the
 * accumulator true and folds with logical-and; or seeds false and folds with logical-or. Evaluation
 * short-circuits: it returns as soon as the accumulator flips away from the seed value, or when the children
 * are exhausted.
 *
 * thread+16 = active stack frame; frame+4 = current expression; frame+12 = frame size (int16). */

#include <stdint.h>
#include "headers/hs_syntax_node.h"
#include "headers/hs_thread.h"
#include "headers/data_array.h"
#include "headers/hs_function_index.h"
#include "headers/blam_data_globals.h"

extern void hs_evaluate(int thread_index, int expression_index, int *destination);
extern void hs_return(uint16_t thread_index, int value);

void hs_evaluate_logical(int16_t function_index, int thread_index, uint8_t initialize)
{
    hs_thread *thread = (hs_thread *)hs_thread_data->data + (uint16_t)thread_index;

    /* inlined hs_thread_stack_allocate x3: reserve slots at the frame data top, aligned up.
     * The compiled `if (slot - elem > top) slot -= elem` correction can never fire (align-up
     * advances at most 3 bytes; the 1-byte slot needs no alignment); kept verbatim for fidelity. */
    hs_stack_frame *frame = thread->stack;
    unsigned char *stack_top = &frame->data[frame->size];
    int *iterator = (int *)(((uintptr_t)stack_top + 3) & ~(uintptr_t)3) /* align up to 4 */;
    if ( (uintptr_t)(iterator - 1) > (uintptr_t)stack_top )   /* dead branch (shipped) */
        --iterator;
    frame->size = (int16_t)((unsigned char *)iterator - frame->data + 4);

    frame = thread->stack;
    stack_top = &frame->data[frame->size];
    int *operand = (int *)(((uintptr_t)stack_top + 3) & ~(uintptr_t)3) /* align up to 4 */;
    if ( (uintptr_t)(operand - 1) > (uintptr_t)stack_top )   /* dead branch (shipped) */
        --operand;
    frame->size = (int16_t)((unsigned char *)operand - frame->data + 4);

    frame = thread->stack;
    stack_top = &frame->data[frame->size];
    char *accumulator = (char *)stack_top;
    if ( (uintptr_t)(stack_top - 1) > (uintptr_t)accumulator )   /* dead branch (shipped) */
        accumulator = (char *)stack_top - 1;
    frame->size = (int16_t)((unsigned char *)accumulator - frame->data + 1);

    if ( initialize )
    {
        int begin_expression = frame->expression_index;
        *iterator = HS_SYNTAX_NODE(HS_SYNTAX_NODE(begin_expression).data).next_node_index;
        *accumulator = (function_index == _hs_function_and);
    }
    else
    {
        char folded;
        if ( function_index == _hs_function_and )
        {
            folded = 1;
            if ( !*accumulator || !*(char *)operand )
                folded = 0;
        }
        else if ( *accumulator || *(char *)operand )        /* or */
        {
            folded = 1;
        }
        else
        {
            folded = 0;
        }
        *accumulator = folded;
    }

    if ( *iterator == -1 || *accumulator != (function_index == _hs_function_and) )
    {
        int return_value;
        *(char *)&return_value = *accumulator;              /* boolean byte (big-endian high byte) */
        hs_return(thread_index, return_value);
    }
    else
    {
        hs_evaluate(thread_index, *iterator, operand);
        *iterator = HS_SYNTAX_NODE(*iterator).next_node_index;
    }
}
