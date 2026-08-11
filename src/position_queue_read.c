/* position_queue_read @0x837AAE88 — pop the buffered remote-player position matching a given client update
 * id. Peeks the head entry: if its update id matches, the position is returned and the entry consumed.
 * If the head is older than the requested id (within its validity window), nothing is read yet; otherwise
 * the stale head is discarded and the read retried. Returns 1 on success, 0 when not yet available. */

#include <stdint.h>
#include "headers/position_queue.h"
#include "headers/real_point3d.h"

#include "headers/simple_circular_queue.h"
#include "headers/simple_circular_queue.h"
extern uint8_t simple_circular_queue_peek(simple_circular_queue *queue, void **user_data);
extern int simple_circular_queue_read(simple_circular_queue *queue, void **user_data);

uint8_t position_queue_read(position_queue *queue, int client_update_id, real_point3d *server_position_as_of_this_update)
{
    void *entry[12];
    int result = simple_circular_queue_peek(&queue->queue, entry);
    if ((uint8_t)result == 1)
    {
        float *position_entry = (float *)entry[0];
        int queued_update_id = *(int *)entry[0];
        if (queued_update_id == client_update_id)
        {
            server_position_as_of_this_update->n[0] = ((float *)entry[0])[2];
            server_position_as_of_this_update->n[1] = position_entry[3];
            server_position_as_of_this_update->n[2] = position_entry[4];
            return simple_circular_queue_read(&queue->queue, entry);
        }
        else
        {
            int age;
            if (queued_update_id >= client_update_id)
                age = queued_update_id > client_update_id ? queued_update_id - client_update_id : 0;
            else
                age = queued_update_id - client_update_id + 64;

            if (age <= *((int *)entry[0] + 1))
            {
                return 0;
            }
            else
            {
                simple_circular_queue_read(&queue->queue, entry);
                return position_queue_read(queue, client_update_id, server_position_as_of_this_update);
            }
        }
    }
    return result;
}
