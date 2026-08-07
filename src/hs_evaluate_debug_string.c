/* hs_evaluate_debug_string @0x83690590 — runtime that gathers a call's string arguments into a frame-resident
 * pointer array (up to 32 entries, a 128-byte slot). Three frame slots: the child iterator, the collected
 * count, and the 128-byte pointer array. Each pass evaluates the next child string and appends its pointer;
 * once the children are exhausted or 32 strings are collected it returns -1.
 *
 * thread+16 = active frame; frame+4 = current expression; frame+12 = frame size (int16). */

#include <stdint.h>
#include "headers/hs_syntax_node.h"
#include "headers/hs_thread.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void hs_evaluate(int thread_index, int expression_index, int *destination);
extern void hs_return(uint16_t thread_index, int value);
extern void *memset(void *dst, int value, unsigned int count);

void hs_evaluate_debug_string(int16_t function_index, int thread_index, uint8_t initialize)
{
    hs_thread *thread = (hs_thread *)hs_thread_data->data + (uint16_t)thread_index;

    /* inlined hs_thread_stack_allocate x3: reserve slots at the frame data top, 4-aligned up.
     * The compiled `if (slot - elem > top) slot -= elem` correction can never fire (align-up
     * advances at most 3 bytes); kept verbatim for fidelity. */
    hs_stack_frame *frame = thread->stack;
    unsigned char *stack_top = &frame->data[frame->size];
    int *iterator = (int *)(((uintptr_t)stack_top + 3) & ~(uintptr_t)3);
    if ( (uintptr_t)(iterator - 1) > (uintptr_t)stack_top )   /* dead branch (shipped) */
        --iterator;
    frame->size = (int16_t)((unsigned char *)iterator - frame->data + 4);

    frame = thread->stack;
    stack_top = &frame->data[frame->size];
    int *count = (int *)(((uintptr_t)stack_top + 3) & ~(uintptr_t)3);
    if ( (uintptr_t)(count - 1) > (uintptr_t)stack_top )   /* dead branch (shipped) */
        --count;
    frame->size = (int16_t)((unsigned char *)count - frame->data + 4);

    frame = thread->stack;
    stack_top = &frame->data[frame->size];
    char *strings = (char *)(((uintptr_t)stack_top + 3) & ~(uintptr_t)3);
    if ( (uintptr_t)(strings - 4) > (uintptr_t)stack_top )   /* dead branch (shipped) */
        strings -= 4;
    frame->size = (int16_t)((unsigned char *)strings - frame->data + 128);

    if ( initialize )
    {
        int begin_expression = frame->expression_index;
        *iterator = HS_SYNTAX_NODE(HS_SYNTAX_NODE(begin_expression).data).next_node_index;
        *count = 0;
        memset(strings, 0, 0x80u);
    }

    if ( *iterator == -1 || *count >= 32 )
    {
        hs_return(thread_index, -1);
    }
    else
    {
        int evaluated_string;
        hs_evaluate(thread_index, *iterator, &evaluated_string);
        *iterator = HS_SYNTAX_NODE(*iterator).next_node_index;
        ((int *)strings)[(*count)++] = evaluated_string;   /* 4-byte slots in the 4-aligned frame array */
    }
}
