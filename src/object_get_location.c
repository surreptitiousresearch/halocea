/* object_get_location @ 0x836EF558 — the BSP location (leaf + cluster) of an object, taken from its
 * ultimate parent (so attached objects share the carrier's location).
 *
 * Deviation: the decompiler lost the assignment of the output pointer (an uninitialised local mapped
 * to the `location` out-parameter); restored here to write through `location`. */

#include <stdint.h>
#include "headers/location.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/blam_data_globals.h"

extern int object_get_ultimate_parent(int object_index);

location *object_get_location(int object_index, location *location_out)
{
    int parent = (uint16_t)object_get_ultimate_parent(object_index);
    object_datum *object =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, parent)->datum;

    /* recovered: *(int*)location / *((int*)location+1) -> named location members. */
    location_out->leaf_index    = object->object.location.leaf_index;
    location_out->cluster_index = object->object.location.cluster_index;
    location_out->bonus         = object->object.location.bonus;
    return location_out;
}
