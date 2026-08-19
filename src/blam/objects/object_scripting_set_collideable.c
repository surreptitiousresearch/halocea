/* object_scripting_set_collideable @0x836EDBA8 — enable or disable an object's collision (object flags +0x10
 * bit 0x1000000: set = non-collideable). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/object_flags.h"
#include "headers/blam_data_globals.h"


void object_scripting_set_collideable(int object_index, uint8_t collideable)
{
    if ( object_index == -1 )
        return;
    object_datum *object_data = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    int flags = object_data->object.flags;
    object_data->object.flags = collideable ? (flags & ~(1u << _object_no_collisions_bit))
                                            : (flags | (1u << _object_no_collisions_bit));
}
