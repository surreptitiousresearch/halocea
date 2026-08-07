/* hs_evaluate_arithmetic @0x8368F648 — runtime of the variadic arithmetic operators (+ - * / min max). Four
 * frame slots are pushed: a 2-byte operand index, the child iterator, a per-operand value slot, and the float
 * accumulator. Each pass evaluates the next child into the value slot and folds it into the accumulator with
 * the operator selected by function_index (7=+ 8=- 9=* 10=/ 11=min 12=max); the first operand seeds the
 * accumulator. When the children are exhausted the accumulated float is returned (by bit pattern).
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

void hs_evaluate_arithmetic(int16_t function_index, int thread_index, uint8_t initialize)
{
    hs_thread *thread = (hs_thread *)hs_thread_data->data + (uint16_t)thread_index;

    /* inlined hs_thread_stack_allocate x4: reserve slots at the frame data top, aligned up.
     * The compiled `if (slot - elem > top) slot -= elem` correction can never fire (align-up
     * advances at most 3 bytes); kept verbatim for fidelity. */
    hs_stack_frame *frame = thread->stack;
    unsigned char *stack_top = &frame->data[frame->size];
    int16_t *operand_index = (int16_t *)(((uintptr_t)stack_top + 1) & ~(uintptr_t)0x1);
    if ( (uintptr_t)(operand_index - 1) > (uintptr_t)stack_top )   /* dead branch (shipped) */
        --operand_index;
    frame->size = (int16_t)((unsigned char *)operand_index - frame->data + 2);

    frame = thread->stack;
    stack_top = &frame->data[frame->size];
    int *iterator = (int *)(((uintptr_t)stack_top + 3) & ~(uintptr_t)0x3);
    if ( (uintptr_t)(iterator - 1) > (uintptr_t)stack_top )   /* dead branch (shipped) */
        --iterator;
    frame->size = (int16_t)((unsigned char *)iterator - frame->data + 4);

    frame = thread->stack;
    stack_top = &frame->data[frame->size];
    int *operand = (int *)(((uintptr_t)stack_top + 3) & ~(uintptr_t)0x3);
    if ( (uintptr_t)(operand - 1) > (uintptr_t)stack_top )   /* dead branch (shipped) */
        --operand;
    frame->size = (int16_t)((unsigned char *)operand - frame->data + 4);

    frame = thread->stack;
    stack_top = &frame->data[frame->size];
    float *accumulator = (float *)(((uintptr_t)stack_top + 3) & ~(uintptr_t)0x3);
    if ( (uintptr_t)(accumulator - 1) > (uintptr_t)stack_top )   /* dead branch (shipped) */
        --accumulator;
    frame->size = (int16_t)((unsigned char *)accumulator - frame->data + 4);

    if ( initialize )
    {
        *operand_index = 0;
        int begin_expression = frame->expression_index;
        *iterator = HS_SYNTAX_NODE(HS_SYNTAX_NODE(begin_expression).data).next_node_index;
        goto step;
    }

    {
        float value = *(float *)operand;
        if ( !*operand_index )
            goto seed;
        if ( function_index >= _hs_function_plus && function_index <= _hs_function_max )
        {
            switch ( function_index )
            {
                case _hs_function_plus:   *accumulator = *accumulator + *(float *)operand; goto advance;
                case _hs_function_minus:  *accumulator = *accumulator - *(float *)operand; goto advance;
                case _hs_function_times:  *accumulator = *accumulator * *(float *)operand; goto advance;
                case _hs_function_divide: *accumulator = *accumulator / *(float *)operand; goto advance;
                case _hs_function_min: if ( *accumulator <= value ) value = *accumulator; break;
                default: if ( *accumulator > value ) value = *accumulator; break;   /* _hs_function_max */
            }
seed:
            *accumulator = value;
        }
advance:
        ++*operand_index;
    }

step:
    if ( *iterator == -1 )
    {
        hs_return(thread_index, *(int *)accumulator);
    }
    else
    {
        hs_evaluate(thread_index, *iterator, operand);
        *iterator = HS_SYNTAX_NODE(*iterator).next_node_index;
    }
}
