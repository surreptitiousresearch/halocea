/* hs_stack_push @0x8368D1D0 — push a new HaloScript interpreter stack frame immediately past the current
 * frame's used space, link it to its parent, and make it current with an empty body. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/hs_thread.h"
#include "headers/hs_stack_frame.h"
#include "headers/blam_data_globals.h"

void hs_stack_push(uint16_t thread_index)
{
    hs_thread *thread = DATA_ARRAY_ELEMENT(hs_thread_data, hs_thread, thread_index);
    hs_stack_frame *frame = thread->stack;

    /* new frame begins 16 bytes past the current frame's end (frame + frame->size) */
    hs_stack_frame *new_frame = (hs_stack_frame *)((char *)frame + frame->size + 16);
    new_frame->parent = frame;
    thread->stack = new_frame;
    new_frame->size = 0;
}
