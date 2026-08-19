/* simple_circular_queue_read @0x837AB350 — dequeue the head user-data pointer. Sets *user_data and returns
 * 1 when non-empty (advancing read_position with wraparound), else clears *user_data and returns 0. */

#include <stdint.h>
#include "headers/simple_circular_queue.h"

int simple_circular_queue_read(simple_circular_queue *queue, void **user_data)
{
    int read_position = queue->read_position;
    int result;
    if ( read_position == queue->write_position )
    {
        result = 0;
        *user_data = 0;
    }
    else
    {
        result = 1;
        *user_data = queue->entries[read_position].user_data;
    }
    if ( (uint8_t)result == 1 )
        queue->read_position = (queue->read_position + 1) % queue->max_size;
    return result;
}
