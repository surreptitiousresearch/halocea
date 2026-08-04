/* action_queue_initialize @0x837AAF74 — allocate the action queue's entry bank (0x14A0 bytes =
 * 120 entries x 44 bytes), zero it, wire it into the circular queue, and clear the
 * last-valid-action flag. */

#include "headers/action_queue.h"

#include "headers/simple_circular_queue.h"
#include "headers/simple_circular_queue.h"
extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
extern void *memset(void *dst, int value, unsigned int count);
extern void simple_circular_queue_initialize(simple_circular_queue *queue, int max_queue_size, void *user_data_array, int user_data_element_size);

void action_queue_initialize(action_queue *queue)
{
    action_entry *entry_bank = dlMalloc(0x14A0u, "D:\\Projects\\code\\HCEX\\sources\\game\\action_queue.c", 0x1Bu);
    queue->entry_bank = entry_bank;
    memset(entry_bank, 0, 0x14A0u);
    simple_circular_queue_initialize(&queue->queue, 120, queue->entry_bank, 44);
    queue->has_last_valid_action = 0;
}
