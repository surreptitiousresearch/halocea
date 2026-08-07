/* hs_evaluate_sleep_until @0x8368FF20 — runtime of (sleep_until <condition> [<period> [<timeout>]]). Five frame
 * slots: the condition result, the poll period (default 30 ticks), the timeout, the start time, and a 2-byte
 * phase flag. Initialization records the start time and evaluates the optional period; the optional timeout is
 * evaluated next; thereafter each poll re-evaluates the condition and, if neither the condition nor the timeout
 * is satisfied, reschedules the thread to wake one period later (clamped to the timeout).
 *
 * thread+8 = wake time. thread+16 = active frame; frame+4 = current expression; frame+12 = frame size (int16).
 * Raw frame bump allocations are reproduced verbatim. */

#include <stdint.h>
#include "headers/hs_syntax_node.h"
#include "headers/hs_thread.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void hs_evaluate(int thread_index, int expression_index, int *destination);
extern void hs_return(uint16_t thread_index, int value);
extern int game_time_get(void);

void hs_evaluate_sleep_until(int16_t function_index, int thread_index, uint8_t initialize)
{
    hs_thread *thread = (hs_thread *)hs_thread_data->data + (uint16_t)thread_index;

    /* inlined hs_thread_stack_allocate x5: reserve slots at the frame data top, aligned up.
     * The compiled `if (slot - 1 > top) --slot` correction can never fire (align-up advances at
     * most 3 bytes); kept verbatim for fidelity. */
    hs_stack_frame *frame = thread->stack;
    unsigned char *stack_top = &frame->data[frame->size];
    int *condition_slot = (int *)(((uintptr_t)stack_top + 3) & ~(uintptr_t)0x3);
    if ( (uintptr_t)(condition_slot - 1) > (uintptr_t)stack_top )   /* dead branch (shipped) */
        --condition_slot;
    frame->size = (int16_t)((unsigned char *)condition_slot - frame->data + 4);

    frame = thread->stack;
    stack_top = &frame->data[frame->size];
    int *period_slot = (int *)(((uintptr_t)stack_top + 3) & ~(uintptr_t)0x3);
    if ( (uintptr_t)(period_slot - 1) > (uintptr_t)stack_top )   /* dead branch (shipped) */
        --period_slot;
    frame->size = (int16_t)((unsigned char *)period_slot - frame->data + 4);

    frame = thread->stack;
    stack_top = &frame->data[frame->size];
    int *timeout_slot = (int *)(((uintptr_t)stack_top + 3) & ~(uintptr_t)0x3);
    if ( (uintptr_t)(timeout_slot - 1) > (uintptr_t)stack_top )   /* dead branch (shipped) */
        --timeout_slot;
    frame->size = (int16_t)((unsigned char *)timeout_slot - frame->data + 4);

    frame = thread->stack;
    stack_top = &frame->data[frame->size];
    int *start_time = (int *)(((uintptr_t)stack_top + 3) & ~(uintptr_t)0x3);
    if ( (uintptr_t)(start_time - 1) > (uintptr_t)stack_top )   /* dead branch (shipped) */
        --start_time;
    frame->size = (int16_t)((unsigned char *)start_time - frame->data + 4);

    frame = thread->stack;
    stack_top = &frame->data[frame->size];
    int16_t *phase = (int16_t *)(((uintptr_t)stack_top + 1) & ~(uintptr_t)0x1);
    if ( (uintptr_t)(phase - 1) > (uintptr_t)stack_top )   /* dead branch (shipped) */
        --phase;
    frame->size = (int16_t)((unsigned char *)phase - frame->data + 2);

    int sleep_expression = frame->expression_index;
    int condition = HS_SYNTAX_NODE(HS_SYNTAX_NODE(sleep_expression).data).next_node_index;
    int period_argument = HS_SYNTAX_NODE(condition).next_node_index;

    if ( initialize )
    {
        *(char *)condition_slot = 0;
        *start_time = game_time_get();
        *phase = 0;
        *(int16_t *)period_slot = 30;
        *timeout_slot = -1;
        if ( period_argument != -1 )
        {
            hs_evaluate(thread_index, period_argument, period_slot);
            return;
        }
    }

    int timeout_argument = -1;
    unsigned char poll;
    if ( *phase )
    {
        poll = 1;
    }
    else
    {
        *phase = 1;
        if ( period_argument == -1 )
        {
            poll = 1;
        }
        else
        {
            timeout_argument = HS_SYNTAX_NODE(period_argument).next_node_index;
            poll = (timeout_argument == -1);
        }
    }

    if ( !poll )
    {
        hs_evaluate(thread_index, timeout_argument, timeout_slot);
        return;
    }

    if ( *phase == 1 )
    {
        if ( *(char *)condition_slot
          || (*timeout_slot != -1 && game_time_get() >= *start_time + *timeout_slot) )
        {
            hs_return(thread_index, 0);
        }
        else
        {
            hs_evaluate(thread_index, condition, condition_slot);
            int period = (*(int16_t *)period_slot >= 1) ? *(int16_t *)period_slot : 1;
            int wake = game_time_get() + period;
            thread->sleep_until = wake;
            if ( *timeout_slot != -1 )
            {
                if ( *timeout_slot + *start_time <= wake )
                    wake = *timeout_slot + *start_time;
                thread->sleep_until = wake;
            }
        }
    }
}
