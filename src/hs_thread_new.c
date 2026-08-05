/* hs_thread_new @0x8368CFE0 — allocate a new HaloScript thread datum (stride 536) and initialize it:
 * point the thread's stack-frame pointer (+16) at its embedded local-storage block (+24), clear the
 * stack depth (+24), seed the first frame header (size 0 @+12, return -1 @+4), store the thread type
 * (+2) and script index (+4), clear the wake byte (+3), and set the sleep word (+8) to -2 when the
 * bound script is a dormant script (hs_scripts entry, type word @ word-offset 16, == 1; type 0 is startup).
 * Raw offsets reproduced from the decompiler — there is no hs_thread struct header. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/hs_thread.h"
#include "headers/hs_script.h"
#include "headers/scenario.h"
#include "headers/hs_script_type.h"
#include "headers/hs_sleep.h"
#include "headers/blam_data_globals.h"

extern int datum_new(data_array *data);

int hs_thread_new(int16_t type, int script_index)
{
    int thread_index = datum_new(hs_thread_data);
    if ( thread_index != -1 )
    {
        int sleep_until = 0;
        hs_thread *thread = DATA_ARRAY_ELEMENT(hs_thread_data, hs_thread, thread_index);
        thread->stack = (hs_stack_frame *)thread->stack_data;
        thread->stack->parent = 0;
        thread->stack->size = 0;
        thread->stack->expression_index = -1;
        thread->type = (uint8_t)type;
        thread->script_index = script_index;
        thread->flags = 0;
        if ( script_index != -1
          && ((hs_script *)global_scenario->hs_scripts.address)[script_index].script_type == _hs_script_dormant )
            sleep_until = HS_SLEEP_INDEFINITE;
        thread->sleep_until = sleep_until;
    }
    return thread_index;
}
