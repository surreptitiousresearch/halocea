/* objects_reconnect_to_structure_bsp @0x836F0F40 — after a structure BSP swap, find every live object
 * flagged connected-to-map (_object_connected_to_map_bit, 0x800) with no parent, clear that flag, invalidate its cached
 * BSP-slot cluster reference, and recompute its cluster location: first try scenario_location_from_point
 * against its bounding sphere; if that fails (no cluster), sweep a collision-BSP query sphere around the
 * bounding sphere and use the first overlapping leaf's cluster (falling back to the object's plain
 * position if the sphere test itself finds nothing). Reconnects the object to the map at the resolved
 * location. Returns whatever object_iterator_next() last returned (nullptr once exhausted).
 *
 * The per-slot write clears the object_header_datum.cluster_index field (byte +4 of the 12-byte header
 * element), expressed via the canonical DATA_ARRAY_ELEMENT(...)->cluster_index form. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_iterator.h"
#include "headers/object_datum.h"
#include "headers/location.h"
#include "headers/real_point3d.h"
#include "headers/structure_bsp.h"
#include "headers/structure_leaf.h"
#include "headers/collision_bsp.h"
#include "headers/collision_bsp_test_sphere_result.h"
#include "headers/object_flags.h"
#include "headers/blam_data_globals.h"


#include "headers/collision_bsp.h"
#include "headers/real_point3d.h"
extern void *object_iterator_next(object_iterator *iterator);
extern void scenario_location_from_point(location *location, const real_point3d *point);
extern uint8_t collision_bsp_test_sphere(const collision_bsp *bsp, int16_t breakable_surface_count, const uint8_t *breakable_surface_flags, const real_point3d *center, float radius, collision_bsp_test_sphere_result *result);
extern void object_reconnect_to_map(int object_index, const location *location_in);

void *objects_reconnect_to_structure_bsp(void)
{
    object_iterator iterator;
    iterator.flags = 0;
    iterator.absolute_index = 0;
    iterator.signature = -2038004090;
    iterator.type_flags = -1;
    iterator.index = -1;

    object_datum *object;
    for ( object = object_iterator_next(&iterator); object;
          object = object_iterator_next(&iterator) )
    {
        unsigned int flags = object->object.flags;
        if ( (flags & (1u << _object_connected_to_map_bit)) == 0 || object->object.parent_object_index != -1 )
            continue;

        object->object.location.cluster_index = -1;   /* invalidated ahead of the recompute below */

        int index = iterator.index;
        object->object.flags = flags & ~(1u << _object_connected_to_map_bit);
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, index)->cluster_index = -1;

        location leaf_location;
        scenario_location_from_point(&leaf_location, &object->object.bounding_sphere_center);

        if ( (unsigned __int16)leaf_location.cluster_index == 0xFFFF )
        {
            collision_bsp_test_sphere_result sphere_result;
            collision_bsp_test_sphere(global_collision_bsp, 0, nullptr, &object->object.bounding_sphere_center,
                object->object.bounding_sphere_radius, &sphere_result);

            if ( sphere_result.leaf_count )
            {
                int leaf_index = sphere_result.leaf_indices[0];
                leaf_location.leaf_index = leaf_index;
                leaf_location.cluster_index = (leaf_index == -1) ? -1
                    : ((const structure_leaf *)global_structure_bsp->leaves.address)[leaf_index].cluster_index;
            }
            else
            {
                scenario_location_from_point(&leaf_location, &object->object.position);
            }
        }

        object_reconnect_to_map(index, &leaf_location);
    }

    return object;
}
