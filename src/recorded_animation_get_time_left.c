/* recorded_animation_get_time_left @0x837125A0 — ticks remaining on the recorded animation playing on a
 * unit, or 0 if none (animation_thread.unit_index / .ticks_left). */

#include "headers/animation_thread.h"
#include "headers/data_iterator.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"


int recorded_animation_get_time_left(int unit_index)
{
    data_iterator iterator;
    data_iterator_new(&iterator, animation_threads);
    animation_thread *thread = data_iterator_next(&iterator);
    if ( !thread )
        return 0;
    while ( thread->unit_index != unit_index )
    {
        thread = data_iterator_next(&iterator);
        if ( !thread )
            return 0;
    }
    return thread->ticks_left;
}
