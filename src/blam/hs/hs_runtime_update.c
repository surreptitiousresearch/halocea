/* hs_runtime_update @0x83690C48 — per-frame HaloScript runtime tick. When initialized, run every thread
 * whose wake time (thread +8) has arrived (>= 0 and <= now), tracking whether any type-2 (persistent) thread
 * exists. Afterwards collect dead object-list references, and — only when no persistent thread is active and
 * every 16 ticks — collect dead script nodes. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/hs_thread.h"

#include "headers/hs_runtime_globals.h"
#include "headers/hs_thread_type.h"
#include "headers/blam_data_globals.h"
#include "headers/data_array.h"
extern int game_time_get(void);
extern int data_next_index(const data_array *data, int16_t index);
extern void hs_thread_main(int thread_index);
extern void object_list_gc(void);
extern void hs_node_gc(void);

void hs_runtime_update(void)
{
    if ( hs_runtime_globals.initialized )
    {
        int now = game_time_get();
        char persistent_thread_active = 0;
        for ( int i = data_next_index(hs_thread_data, -1); hs_runtime_globals.initialized; i = data_next_index(hs_thread_data, i) )
        {
            if ( i == -1 )
                break;
            hs_thread *thread = DATA_ARRAY_ELEMENT(hs_thread_data, hs_thread, i);
            if ( thread->type == hs_thread_type_runtime_evaluate )
                persistent_thread_active = 1;
            int wake_time = thread->sleep_until;
            if ( wake_time >= 0 && wake_time <= now )
                hs_thread_main(i);
        }
        object_list_gc();
        if ( !persistent_thread_active && !(game_time_get() % 16) )
            hs_node_gc();
    }
}
