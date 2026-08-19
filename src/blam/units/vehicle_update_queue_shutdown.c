/* vehicle_update_queue_shutdown @0x837B46A0 */
#include "headers/vehicle_update_queue.h"

extern void simple_circular_queue_shutdown(simple_circular_queue *queue);
extern void dlFree(void *ptr);

void vehicle_update_queue_shutdown(vehicle_update_queue *queue)
{
    simple_circular_queue_shutdown(&queue->queue);
    dlFree(queue->entry_bank);
    queue->entry_bank = 0;
}
