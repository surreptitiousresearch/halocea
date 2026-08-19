/* object_translate @0x836F1658 — move an object to a new world position, updating its map connectivity:
 * disconnect from the map, write the new position into the object's transform (+92), then reconnect at the
 * supplied location (BSP/cluster). */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/real_point3d.h"
#include "headers/location.h"
#include "headers/blam_data_globals.h"

extern void object_disconnect_from_map(int object_index);
extern void object_reconnect_to_map(int object_index, const location *location_in);

void object_translate(int object_index, const real_point3d *new_position, const location *new_location)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    object_disconnect_from_map(object_index);
    object->object.position = *new_position;  /* +92 = object_datum.object.position (0x04 + 0x58) */
    object_reconnect_to_map(object_index, new_location);
}
