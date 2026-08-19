/* simple_circular_queue_get_number_of_queued_entries @0x837AB250 — count entries currently held in
 * a simple circular queue, accounting for wraparound. */

#include "headers/simple_circular_queue.h"

int simple_circular_queue_get_number_of_queued_entries(const simple_circular_queue *queue)
{
    int write_position = queue->write_position;
    int read_position = queue->read_position;
    if ( write_position > read_position )
        return write_position - read_position;
    if ( write_position == read_position )
        return 0;
    return write_position - read_position + queue->max_size;
}
