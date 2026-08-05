/* hs_evaluate_sleep @0x8368FC68 — runtime of (sleep <ticks> [<script>]). Three frame slots: the tick count, the
 * optional target-script index, and a 2-byte phase flag. Pass 1 evaluates the tick count; pass 2 evaluates the
 * optional script name (defaulting the target to the current thread); pass 3 reschedules the target thread to
 * wake `ticks` game-ticks from now (a negative tick count parks it indefinitely, marker -2), saving and marking
 * the prior wake state when suspending another thread.
 *
 * Thread layout: thread+8 = wake time; thread+12 = saved wake time; thread[3] = flags (bit 2 = suspended).
 * thread+16 = active frame; frame+4 = current expression; frame+12 = frame size (int16). */

#include <stdint.h>
#include "headers/hs_syntax_node.h"
#include "headers/hs_thread.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void hs_evaluate(int thread_index, int expression_index, int *destination);
extern void hs_return(uint16_t thread_index, int value);
extern int hs_find_thread_by_script(int16_t script_index);
extern int game_time_get(void);

void hs_evaluate_sleep(int16_t function_index, int thread_index, uint8_t initialize)
{
    hs_thread *thread = (hs_thread *)hs_thread_data->data + (uint16_t)thread_index;

    /* inlined hs_thread_stack_allocate x3: reserve slots at the frame data top, aligned up.
     * The compiled `if (slot - 1 > top) --slot` correction can never fire (align-up advances at
     * most 3 bytes); kept verbatim for fidelity. */
    hs_stack_frame *frame = thread->stack;
    unsigned char *stack_top = &frame->data[frame->size];
    int *ticks_slot = (int *)(((unsigned int)stack_top + 3) & ~0x3u);
    if ( (unsigned int)(ticks_slot - 1) > (unsigned int)stack_top )   /* dead branch (shipped) */
        --ticks_slot;
    frame->size = (int16_t)((unsigned char *)ticks_slot - frame->data + 4);

    frame = thread->stack;
    stack_top = &frame->data[frame->size];
    int *script_slot = (int *)(((unsigned int)stack_top + 3) & ~0x3u);
    if ( (unsigned int)(script_slot - 1) > (unsigned int)stack_top )   /* dead branch (shipped) */
        --script_slot;
    frame->size = (int16_t)((unsigned char *)script_slot - frame->data + 4);

    frame = thread->stack;
    stack_top = &frame->data[frame->size];
    int16_t *phase = (int16_t *)(((unsigned int)stack_top + 1) & ~0x1u);
    if ( (unsigned int)(phase - 1) > (unsigned int)stack_top )   /* dead branch (shipped) */
        --phase;
    int target_thread = thread_index;
    frame->size = (int16_t)((unsigned char *)phase - frame->data + 2);

    int sleep_expression = frame->expression_index;
    int name_node = HS_SYNTAX_NODE(sleep_expression).data;
    int ticks_argument = HS_SYNTAX_NODE(name_node).next_node_index;

    if ( initialize )
    {
        hs_evaluate(thread_index, ticks_argument, ticks_slot);
        *phase = 0;
        return;
    }

    if ( !*phase )
    {
        int script_argument = HS_SYNTAX_NODE(ticks_argument).next_node_index;
        *phase = 1;
        if ( script_argument != -1 )
        {
            hs_evaluate(thread_index, script_argument, script_slot);
            return;
        }
        *script_slot = -1;
    }

    if ( *phase )
    {
        int16_t ticks = (int16_t)*ticks_slot;
        if ( *(int16_t *)ticks_slot )
        {
            uint16_t script_index = (uint16_t)*script_slot;
            if ( script_index != 0xFFFF )
                target_thread = hs_find_thread_by_script(script_index);
            if ( target_thread != -1 )
            {
                hs_thread *target = (hs_thread *)hs_thread_data->data + (uint16_t)target_thread;
                int wake_time = (ticks >= 0) ? game_time_get() + ticks : -2;
                int current_wake = target->sleep_until;
                if ( current_wake != -1 )
                {
                    if ( target_thread != thread_index )
                    {
                        unsigned char flags = target->flags;
                        if ( (flags & 2) == 0 )
                        {
                            target->latent_sleep_until = current_wake;
                            target->flags = flags | 2;
                        }
                    }
                    target->sleep_until = wake_time;
                }
            }
        }
        hs_return(thread_index, 0);
    }
}
