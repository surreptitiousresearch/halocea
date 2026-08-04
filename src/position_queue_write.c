/* position_queue_write @0x837AAE38 — push a position update onto a position_queue. Packs a 20-byte record
 * {client_update_id, updates_until_useful, position.x, position.y, position.z} matching the layout
 * position_queue_read unpacks (entry[0]=id, entry[1]=age threshold, entry[2..4]=position floats).
 *
 * DEVIATION: the decompiler merged the real_point3d-by-value parameter with the leading queue pointer into
 * a bogus 64-bit `server_position_as_of_this_update` and read the queue pointer back out via HIDWORD();
 * DB prototype gives the real signature (queue, position-by-value, client_update_id, updates_until_useful). */

#include <stdint.h>
#include "headers/position_queue.h"
#include "headers/real_point3d.h"
#include "headers/position_queue_write_record.h"

#include "headers/simple_circular_queue.h"
extern int simple_circular_queue_write(simple_circular_queue *queue, void *user_data);

uint8_t position_queue_write(position_queue *queue, real_point3d server_position_as_of_this_update, int client_update_id, int updates_until_useful)
{
    position_queue_write_record record;

    record.client_update_id = client_update_id;
    record.updates_until_useful = updates_until_useful;
    record.position = server_position_as_of_this_update;

    return simple_circular_queue_write(&queue->queue, &record);
}
