/* simple_circular_queue_initialize @0x837AB168 — allocate the queue's entry array (one 4-byte pointer per
 * slot) and point each entry at its slot in the caller-supplied user-data array. Resets read/write
 * positions to empty. */

#include "headers/simple_circular_queue.h"

extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);

/* user_data_array respelled char*->void* 2026-07-30: generic element bank, callers pass typed banks (C4133) */
void simple_circular_queue_initialize(simple_circular_queue *queue, int max_queue_size, void *user_data_array, int user_data_element_size)
{
    simple_circular_queue_entry *entries = dlMalloc(
        4 * max_queue_size, "D:\\Projects\\code\\HCEX\\sources\\game\\simple_circular_queue.c", 0x1Bu);

    queue->entries = entries;
    entries->user_data = 0;
    queue->max_size = max_queue_size;
    queue->user_data_element_size = user_data_element_size;
    queue->read_position = 0;
    queue->write_position = 0;

    if ( max_queue_size > 0 )
    {
        char *slot = (char *)user_data_array;
        for ( int i = 0; i < max_queue_size; ++i )
        {
            queue->entries[i].user_data = slot;
            slot += user_data_element_size;
        }
    }
}
