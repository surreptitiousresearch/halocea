/* gearbox_object_translate @0x836F2110 — move an object directly to a new position, recomputing its world
 * location: resolve the cluster/leaf for the point, disconnect from the map, write the new origin (+92),
 * and reconnect at the resolved location. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/location.h"
#include "headers/object_datum.h"
#include "headers/real_point3d.h"
#include "headers/blam_data_globals.h"


extern void scenario_location_from_point(location *location, const real_point3d *point);
extern void object_disconnect_from_map(int object_index);
extern void object_reconnect_to_map(int object_index, const location *location_in);

void gearbox_object_translate(int object_index, const real_point3d *new_position)
{
    location object_location;
    scenario_location_from_point(&object_location, new_position);

    object_datum *object = (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum);
    object_disconnect_from_map(object_index);
    object->object.position = *new_position;   /* object+92 = _object_datum.position */
    object_reconnect_to_map(object_index, &object_location);
}
