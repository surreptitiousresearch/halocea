/* hs_evaluate @0x8368E5A0 — evaluate one HaloScript expression node on a thread, writing the result to
 * *destination. Primitive nodes resolve immediately: global references read the global (casting from its real
 * type to the node's requested type); literals cast their stored value. Non-primitive (group) nodes can't be
 * evaluated inline, so a new stack frame is pushed on the thread to drive their evaluation, recording the
 * destination and the expression to run.
 *
 * Threads are 536-byte data; thread+16 holds the current stack-frame pointer; each frame keeps its size as an
 * int16 at frame+12. Raw frame offsets are reproduced faithfully from the compiled code. */

#include <stdint.h>
#include "headers/hs_syntax_node.h"
#include "headers/hs_thread.h"
#include "headers/data_array.h"
#include "headers/hs_thread_flags.h"
#include "headers/hs_syntax_node_flags.h"
#include "headers/blam_data_globals.h"

extern int hs_global_evaluate(int16_t global_designator);
extern int16_t hs_global_get_type(int16_t designator);
extern int hs_cast(int thread_index, int16_t actual_type, int16_t desired_type, int value);

void hs_evaluate(int thread_index, int expression_index, int *destination)
{
    hs_thread *thread = DATA_ARRAY_ELEMENT(hs_thread_data, hs_thread, thread_index);
    hs_syntax_node *node = &HS_SYNTAX_NODE(expression_index);
    int16_t flags = node->flags;

    if ( (flags & (1u << _hs_syntax_node_primitive_bit)) != 0 )             /* primitive */
    {
        if ( (flags & (1u << _hs_syntax_node_variable_bit)) != 0 )          /* global/variable reference */
        {
            int value = hs_global_evaluate(node->data);
            int16_t requested_type = node->type;
            int16_t global_type = hs_global_get_type(node->data);
            *destination = hs_cast(thread_index, global_type, requested_type, value);
        }
        else                            /* literal */
        {
            *destination = hs_cast(thread_index, node->___u1.constant_type, node->type, node->data);
        }
    }
    else                                /* non-primitive: push an evaluation frame */
    {
        thread->stack->child_result = destination;
        hs_stack_frame *frame = thread->stack;
        /* the next frame begins after this frame's data, 16-byte header stride as in the binary */
        hs_stack_frame *new_frame = (hs_stack_frame *)((char *)frame + frame->size + 16);
        new_frame->parent = frame;               /* link previous frame */
        thread->stack = new_frame;
        new_frame->size = 0;
        thread->flags |= (1u << _hs_thread_in_function_call_bit);
        new_frame->expression_index = expression_index;
    }
}
