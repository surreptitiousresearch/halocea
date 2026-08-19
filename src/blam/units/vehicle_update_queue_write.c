/* vehicle_update_queue_write @0x837B46E0 — tail-call forwarder (`b simple_circular_queue_write`;
 * confirmed via disasm, not a bare blr). DEVIATION: the DB's own declared parameter types
 * (`vehicle_update_queue *queue, vehicle_update_entry *entry`) differ from the raw jump target's own
 * signature (`simple_circular_queue *queue, void *user_data`) — vehicle_update_queue is simply a
 * layout-compatible specialization of the generic simple_circular_queue that this build reuses
 * directly via a raw tail-branch (no register shuffling), so the pointers are passed through with an
 * explicit cast rather than re-deriving simple_circular_queue's own field layout here.
 * simple_circular_queue_write itself is not yet decompiled — declared extern with its own DB
 * prototype. */

#include <stdint.h>
#include "headers/vehicle_update_queue.h"
#include "headers/vehicle_update_entry.h"
#include "headers/simple_circular_queue.h"

extern int simple_circular_queue_write(simple_circular_queue *queue, void *user_data);

uint8_t vehicle_update_queue_write(vehicle_update_queue *queue, vehicle_update_entry *entry)
{
    return simple_circular_queue_write((simple_circular_queue *)queue, (void *)entry);
}
