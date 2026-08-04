/* simple_circular_queue_peek @0x837AB218 — return the user-data pointer at the head of the queue without
 * dequeuing. Sets *user_data and returns 1 when non-empty (read != write), else clears *user_data and
 * returns 0. The entries array holds 4-byte user-data pointers indexed by read_position. */

#include <stdint.h>
#include "headers/simple_circular_queue.h"

uint8_t simple_circular_queue_peek(simple_circular_queue *queue, void **user_data)
{
    int read_position = queue->read_position;
    if ( read_position == queue->write_position )
    {
        *user_data = 0;
        return 0;
    }
    /* recovered: ((void **)queue->entries)[read_position] -> entries[read_position].user_data */
    *user_data = queue->entries[read_position].user_data;
    return 1;
}
