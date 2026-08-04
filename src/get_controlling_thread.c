/* get_controlling_thread @0x837122D8 — find the animation_thread currently driving a given unit, if any.
 * Returns the thread (or null) and, when thread_index_reference is non-null, writes the thread's datum
 * index (or -1 if none was found). */

#include "headers/animation_thread.h"
#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/blam_data_globals.h"

extern void data_iterator_new(data_iterator *iterator, data_array *data);
extern void *data_iterator_next(data_iterator *iterator);

void * get_controlling_thread(int unit_index, int *thread_index_reference)
{
    int index = -1;
    data_iterator iterator;
    data_iterator_new(&iterator, animation_threads);

    animation_thread *thread = data_iterator_next(&iterator);
    if ( thread )
    {
        while ( thread->unit_index != unit_index )
        {
            thread = data_iterator_next(&iterator);
            if ( !thread )
                goto done;
        }
        index = iterator.index;
    }
done:
    if ( thread_index_reference )
        *thread_index_reference = index;
    return thread;
}
