/* object_activate @ 0x836EBBE0 — mark one object's datum as active (in the PVS / simulated this frame),
 * unless it is already active, its definition forbids activation (flag 0x100000), or it is still
 * parented to another object (parent_object_index != -1). The object datum lives in
 * object_header_data (12-byte object_header_datum elements). */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_flags.h"
#include "headers/object_header_flags.h"
#include "headers/blam_data_globals.h"


void object_activate(int object_index)
{
    object_header_datum *object_header = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index);
    object_datum *object_data = object_header->datum;

    if ( (object_header->flags & (1u << _object_header_active_bit)) == 0
        && (object_data->object.flags & (1u << _object_cannot_be_activated_bit)) == 0
        && object_data->object.parent_object_index == -1 )  /* +284: parent_object_index per DB, not a cluster ref */
    {
        object_header->flags |= (1u << _object_header_active_bit);
    }
}
