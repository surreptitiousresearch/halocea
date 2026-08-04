/* position_queue_shutdown @0x837AAE0C — shut down a position queue: tear down its circular queue and free
 * the entry bank. */

#include "headers/position_queue.h"

extern void simple_circular_queue_shutdown(simple_circular_queue *queue);
extern void dlFree(void *ptr);

void position_queue_shutdown(position_queue *queue)
{
    simple_circular_queue_shutdown(&queue->queue);
    dlFree(queue->entry_bank);
    queue->entry_bank = 0;
}
