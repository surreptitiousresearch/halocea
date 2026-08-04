/* simple_circular_queue_shutdown @0x837AB1F8 — free the queue's entry array and null the pointer. */

#include "headers/simple_circular_queue.h"

extern void dlFree(void *ptr);

void simple_circular_queue_shutdown(simple_circular_queue *queue)
{
    dlFree(queue->entries);
    queue->entries = 0;
}
