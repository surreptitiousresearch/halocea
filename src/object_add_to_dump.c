/* object_add_to_dump @0x836EE4C8 — accumulates one object's stats into a per-type dump_datum: bumps the
 * count and total/maximum datum size, and increments the active/garbage/dead/at-rest tallies from the
 * object's flags. Also counts the object as "outside the map" if its ultimate parent is flagged
 * outside-the-map (0x200000) or has no cluster (cluster index 0xFFFF). */

#include <stdint.h>
#include "headers/dump_datum.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_header_flags.h"
#include "headers/object_datum.h"
#include "headers/object_damage_flags.h"
#include "headers/object_flags.h"
#include "headers/blam_data_globals.h"

extern int object_get_ultimate_parent(int object_index);

void object_add_to_dump(int object_index, dump_datum *dump)
{
    object_header_datum *datum = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index);
    int16_t datum_size = datum->data_size;
    object_datum *object = datum->datum;

    if (datum_size > dump->maximum_size)
        dump->maximum_size = datum_size;
    dump->total_size += datum_size;
    ++dump->count;
    if ((datum->flags & (1u << _object_header_active_bit)) != 0)
        ++dump->active_count;
    if ((object->object.flags & (1u << _object_garbage_bit)) != 0)
        ++dump->garbage_count;
    if ((object->object.damage_flags & (1u << _object_dead_bit)) != 0)
        ++dump->dead_count;
    if ((object->object.flags & (1u << _object_at_rest_bit)) != 0)
        ++dump->at_rest_count;

    uint16_t ultimate_parent = object_get_ultimate_parent(object_index);
    object_datum *parent_object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, ultimate_parent)->datum;
    if ((parent_object->object.flags & (1u << _object_outside_of_map_bit)) != 0
        || (uint16_t)parent_object->object.location.cluster_index == 0xFFFF)
        ++dump->outside_map_count;
}
