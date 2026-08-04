#include "headers/action_queue.h"

int action_queue_get_number_of_queued_ticks(const action_queue *queue)
{
    int read_position = queue->queue.read_position;
    int write_position = queue->queue.write_position;
    int result = 0;

    if ( read_position != write_position )
    {
        simple_circular_queue_entry *entries = queue->queue.entries;

        do
        {
            action_entry *entry = (action_entry *)entries[read_position].user_data;
            result += entry->remaining_ticks_to_apply_action_to;
            read_position = (read_position + 1) % 120;
        } while ( read_position != write_position );
    }
    return result;
}
