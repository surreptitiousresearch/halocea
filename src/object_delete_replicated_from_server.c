/* object_delete_replicated_from_server @0x836F0870 — delete a network-replicated object on request from
 * the server. Validates the object header handle, checks the object's existence/replication-eligibility
 * bit, and (when the object has a live datum) recursively deletes it and its children. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/blam_data_globals.h"


extern void *datum_try_and_get(const data_array *data, int index);
extern void object_delete_initial_recursive(int object_index, uint8_t delete_siblings);

void object_delete_replicated_from_server(int object_index)
{
    object_header_datum *header = datum_try_and_get(object_header_data, object_index);
    if (header && (1 << header->type))
    {
        if (header->datum)
            object_delete_initial_recursive(object_index, 0);
    }
}
