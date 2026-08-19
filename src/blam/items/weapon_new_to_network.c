/* weapon_new_to_network @0x836DBD20 — object_new_to_network entry for weapons: derives the network object
 * flags (bit 5 set only when the object is at rest and the item is not resting on structure)
 * and forwards to weapon_new_to_network_with_flags. Returns bits written. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/object_flags.h"
#include "headers/item_flags.h"
#include "headers/blam_data_globals.h"

extern int weapon_new_to_network_with_flags(int object_index, void *buffer, int buffer_size_in_bits, int object_flags);

int weapon_new_to_network(int object_index, void *buffer, int buffer_size_in_bits)
{
    weapon_datum *object =
        (weapon_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;

    int object_flags = 0;
    if ( (object->object.flags & (1u << _object_at_rest_bit)) != 0
      && (object->item.flags & (1u << _item_on_structure_bit)) == 0 )
        object_flags = 32;

    return weapon_new_to_network_with_flags(object_index, buffer, buffer_size_in_bits, object_flags);
}
