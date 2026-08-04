/* recorded_animation_controlling_unit @0x83712248 — true if an animation_thread is actively driving the
 * given unit: a thread whose unit_index matches and whose "finished" flag (bit 0) is clear. */

#include <stdint.h>
#include "headers/animation_thread.h"
#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/recording_thread_flags.h"
#include "headers/blam_data_globals.h"

extern void data_iterator_new(data_iterator *iterator, data_array *data);
extern void *data_iterator_next(data_iterator *iterator);

uint8_t recorded_animation_controlling_unit(int unit_index)
{
    data_iterator iterator;
    data_iterator_new(&iterator, animation_threads);

    animation_thread *thread = data_iterator_next(&iterator);
    if ( !thread )
        return 0;

    while ( thread->unit_index != unit_index || (thread->flags & (1u << _recording_thread_finished_bit)) != 0 )
    {
        thread = data_iterator_next(&iterator);
        if ( !thread )
            return 0;
    }
    return 1;
}
