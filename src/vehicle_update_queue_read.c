/* vehicle_update_queue_read @0x837B4704 — pop the buffered remote-player vehicle update matching a given
 * client update id. Mirrors position_queue_read: matches the head's update id (copying out the full entry
 * and consuming it), waits when the head is still within its validity window, or discards a stale head and
 * retries. Returns 1 on success, 0 when not yet available. */

#include <stdint.h>
#include "headers/vehicle_update_queue.h"
#include "headers/vehicle_update_entry.h"

#include "headers/simple_circular_queue.h"
#include "headers/simple_circular_queue.h"
extern uint8_t simple_circular_queue_peek(simple_circular_queue *queue, void **user_data);
extern int simple_circular_queue_read(simple_circular_queue *queue, void **user_data);
extern void *memcpy(void *dst, const void *src, unsigned int size);

uint8_t vehicle_update_queue_read(vehicle_update_queue *queue, int client_update_id, vehicle_update_entry *entry)
{
    void *head[12];
    int result = simple_circular_queue_peek(&queue->queue, head);
    if ((unsigned __int8)result == 1)
    {
        vehicle_update_entry *head_entry = (vehicle_update_entry *)head[0];
        /* recovered: *(int *)head[0] -> head_entry->client_update_id */
        int queued_update_id = head_entry->client_update_id;
        if (queued_update_id == client_update_id)
        {
            memcpy(entry, head[0], sizeof(vehicle_update_entry));
            return simple_circular_queue_read(&queue->queue, head);
        }
        else
        {
            int age;
            if (queued_update_id >= client_update_id)
                age = queued_update_id > client_update_id ? queued_update_id - client_update_id : 0;
            else
                age = queued_update_id - client_update_id + 64;

            /* recovered: *((int *)head[0] + 1) -> head_entry->updates_until_useful */
            if (age <= head_entry->updates_until_useful)
            {
                return 0;
            }
            else
            {
                simple_circular_queue_read(&queue->queue, head);
                return vehicle_update_queue_read(queue, client_update_id, entry);
            }
        }
    }
    return result;
}
