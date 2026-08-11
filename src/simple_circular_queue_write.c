/* simple_circular_queue_write @0x837AB298 — enqueue a user element by copying user_data into the slot at
 * write_position and advancing it (with wraparound). Returns 0 (full) when the queue already holds
 * max_size-1 elements, else 1. */

#include <string.h>
#include "headers/simple_circular_queue.h"


int simple_circular_queue_write(simple_circular_queue *queue, void *user_data)
{
    int read_position = queue->read_position;
    int write_position = queue->write_position;
    int used;
    if ( write_position <= read_position )
    {
        if ( write_position >= read_position )
            used = 0;
        else
            used = queue->max_size - read_position + write_position;
    }
    else
    {
        used = write_position - read_position;
    }
    if ( used >= queue->max_size - 1 )
        return 0;
    memcpy(queue->entries[write_position].user_data, user_data, queue->user_data_element_size);
    queue->write_position = (queue->write_position + 1) % queue->max_size;
    return 1;
}
