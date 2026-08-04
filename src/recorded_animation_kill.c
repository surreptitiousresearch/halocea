/* recorded_animation_kill @0x83712378 — flag the recorded-animation thread playing on a unit to stop
 * (set finished + killed bits of animation_thread.flags). No-op if the unit has no recorded animation. */

#include "headers/animation_thread.h"
#include "headers/data_iterator.h"
#include "headers/data_array.h"
#include "headers/recording_thread_flags.h"
#include "headers/blam_data_globals.h"


void recorded_animation_kill(int unit_index)
{
    data_iterator iterator;
    data_iterator_new(&iterator, animation_threads);
    animation_thread *thread = data_iterator_next(&iterator);
    if ( thread )
    {
        while ( thread->unit_index != unit_index )
        {
            thread = data_iterator_next(&iterator);
            if ( !thread )
                return;
        }
        thread->flags |= (1u << _recording_thread_finished_bit) | (1u << _recording_thread_killed_bit);
    }
}
