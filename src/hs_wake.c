/* hs_wake @0x8368D298 — wake a sleeping HS thread. If the thread was externally suspended (flag bit 2) its
 * saved wake time is restored; otherwise the current stack frame is unwound past the yield point. A "yield"
 * frame is identified by the node function_index == _hs_function_sleep_until (20) at the frame's current
 * expression (or its parent frame's expression).
 *
 * thread+8 = wake time (-1 = not sleeping); thread+12 = saved wake time; thread[3] = flags (bit1 = active,
 * bit2 = externally suspended); thread+16 = active stack frame; frame+0 = previous frame link; frame+4 =
 * current expression index. */

#include <stdint.h>
#include "headers/hs_syntax_node.h"
#include "headers/hs_thread.h"
#include "headers/data_array.h"
#include "headers/hs_thread_flags.h"
#include "headers/hs_function_index.h"
#include "headers/hs_sleep.h"
#include "headers/blam_data_globals.h"


void hs_wake(uint16_t thread_index)
{
    hs_thread *thread = (hs_thread *)hs_thread_data->data + thread_index;

    if ( thread->sleep_until == HS_SLEEP_FINISHED )
        return;

    unsigned char flags = thread->flags;
    thread->sleep_until = 0;

    if ( (flags & (1u << _hs_thread_latent_sleep_bit)) != 0 )
    {
        int saved_wake = thread->latent_sleep_until;
        thread->flags = flags & ~(1u << _hs_thread_latent_sleep_bit);
        thread->sleep_until = saved_wake;
        return;
    }

    hs_stack_frame *frame = thread->stack;
    int expression = frame->expression_index;

    /* the yield frame's node is a (sleep_until ...) call; select the function_index union arm
     * (decompiler picked the sibling constant_type arm — same __int16) and name the builtin index */
    if ( expression == -1 || HS_SYNTAX_NODE(expression).___u1.function_index != _hs_function_sleep_until )
    {
        hs_stack_frame *parent = frame->parent;
        if ( parent
          && parent->expression_index != -1
          && HS_SYNTAX_NODE(parent->expression_index).___u1.function_index == _hs_function_sleep_until )
        {
            thread->stack = thread->stack->parent;   /* pop yield frame */
            thread->stack = thread->stack->parent;   /* pop its parent */
            thread->flags &= ~(1u << _hs_thread_in_function_call_bit);
        }
    }
    else
    {
        thread->stack = thread->stack->parent;       /* pop the yield frame */
    }
}
