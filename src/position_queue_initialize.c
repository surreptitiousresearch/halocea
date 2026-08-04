#include "headers/position_queue.h"

#include "headers/simple_circular_queue.h"
#include "headers/simple_circular_queue.h"
extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
extern void *memset(void *dst, int value, unsigned int count);
extern void simple_circular_queue_initialize(simple_circular_queue *queue, int max_queue_size, void *user_data_array, int user_data_element_size);

void position_queue_initialize(position_queue *queue)
{
    void *entry_bank = dlMalloc(0x258u, "D:\\Projects\\code\\HCEX\\sources\\game\\position_queue.c", 0x1Bu);

    queue->entry_bank = entry_bank;
    memset(entry_bank, 0, 0x258u);
    simple_circular_queue_initialize(&queue->queue, 30, queue->entry_bank, 20);
}
