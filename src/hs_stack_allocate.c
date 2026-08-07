/* hs_stack_allocate @0x8368D238 — bump-allocate `size` bytes (aligned to `alignment`) inside the current
 * stack frame of a HaloScript thread and return a pointer to the storage. Advances the frame's size to
 * cover the aligned allocation. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/hs_thread.h"
#include "headers/hs_stack_frame.h"
#include "headers/blam_data_globals.h"

char * hs_stack_allocate(uint16_t thread_index, int16_t size, int alignment)
{
    hs_thread *thread = DATA_ARRAY_ELEMENT(hs_thread_data, hs_thread, thread_index);
    hs_stack_frame *frame = thread->stack;

    char *top = (char *)&frame->data[frame->size];
    /* uintptr_t on BOTH halves: a 32-bit mask zero-extends and clears the high half of the address
     * on x64. The mask keeps the original `-alignment` spelling, sign-extended — `~(alignment-1)`
     * would agree only for powers of two, and silently changing the expression is not the job. */
    char *result = (char *)((uintptr_t)(top + alignment - 1)
                            & (uintptr_t)(intptr_t)(-alignment));
    if ( &result[-alignment] > top )
        result -= alignment;

    frame->size = (int16_t)((result - (char *)frame->data) + size);
    return result;
}
