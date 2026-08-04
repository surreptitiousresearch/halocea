/* hs_stack_pop @0x8368D210 — pop the current HaloScript interpreter stack frame, making the parent
 * frame current again. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/hs_thread.h"
#include "headers/hs_stack_frame.h"
#include "headers/blam_data_globals.h"

void hs_stack_pop(uint16_t thread_index)
{
    hs_thread *thread = DATA_ARRAY_ELEMENT(hs_thread_data, hs_thread, thread_index);
    thread->stack = thread->stack->parent;
}
