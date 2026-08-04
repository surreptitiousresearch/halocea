#include "headers/vehicle_update_queue.h"

#include "headers/simple_circular_queue.h"
#include "headers/simple_circular_queue.h"
extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
extern void *memset(void *dst, int value, unsigned int count);
extern void simple_circular_queue_initialize(simple_circular_queue *queue, int max_queue_size, void *user_data_array, int user_data_element_size);

void vehicle_update_queue_initialize(vehicle_update_queue *queue)
{
    vehicle_update_entry *entry_bank = dlMalloc(
        0x870u, "D:\\Projects\\code\\HCEX\\sources\\game\\vehicle_update_queue.c", 0x1Au);

    queue->entry_bank = entry_bank;
    memset(entry_bank, 0, 0x870u);
    simple_circular_queue_initialize(&queue->queue, 30, queue->entry_bank, 72);
}
