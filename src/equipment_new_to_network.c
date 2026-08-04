/* equipment_new_to_network @0x837B6678 — server-side encoder entry point for a newly-created equipment
 * object: derives the network encode flags from the object's own flags (bit 0x20 at +16) and its item
 * flags (bit 8 at +500), then forwards to equipment_new_to_network_with_flags. Flag value 32 requests
 * encoding of the equipment's "cannot be picked up before" state; it is only requested when the object's
 * own flag is set and its item flag is clear. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/equipment_datum.h"
#include "headers/object_flags.h"
#include "headers/item_flags.h"
#include "headers/blam_data_globals.h"

extern int equipment_new_to_network_with_flags(int object_index, void *buffer, int buffer_size_in_bits, int flags);

int equipment_new_to_network(int object_index, void *buffer, int buffer_size_in_bits)
{
    equipment_datum *object = (equipment_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;

    int flags = 0;
    if ((object->object.flags & (1u << _object_at_rest_bit)) != 0
        && (object->item.flags & (1u << _item_on_structure_bit)) == 0)
        flags = 32;

    return equipment_new_to_network_with_flags(object_index, buffer, buffer_size_in_bits, flags);
}
