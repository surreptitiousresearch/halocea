/* object_reconnect_to_map @0x836F08D8 — re-link an object into the structure BSP cluster/leaf it now
 * occupies and into the appropriate cluster partition, or (for objects parented to another object)
 * splice it onto the parent's child list instead.
 *
 * If the parent index is set, the object is connected through its parent (child-list splice, sets the
 * "connected to parent" header flag 0x80). Otherwise the object is located directly: a caller-supplied
 * location, else scenario_location_from_point at the object's bounding-sphere center falling back to
 * its position. The object flags carry "not in any cluster" (0x200000), collideable (0x2000000),
 * activation-pending (0x80000), plus the permanently set 0x800. cluster_partition selection and
 * PVS-based activation/deletion follow.
 *
 * Deviation: the decompiler typed the flags dword as `const location *`; it is the object's flags,
 * tested for the collideable bit and forwarded to cluster_partition_reconnect.
 * Deviation: the cluster/bonus store was a single int write covering both halfwords; written as the
 * two named halfword assignments. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_header_flags.h"
#include "headers/location.h"
#include "headers/real_point3d.h"
#include "headers/object_flags.h"
#include "headers/blam_data_globals.h"
#include "headers/bit_vector.h"

typedef struct cluster_partition cluster_partition;  /* RAW (irreducible): opaque cluster partition table — no DB layout, forward-decl only */


#include "headers/real_point3d.h"
#include "headers/real_point3d.h"
extern void scenario_location_from_point(location *location, const real_point3d *point);
extern void cluster_partition_reconnect(cluster_partition *partition, int datum_index, int *first_cluster_reference, const real_point3d *position, float radius, const location *location);
extern unsigned int *players_get_combined_pvs(void);
extern void object_delete(int object_index);
extern void object_activate(int object_index);

void object_reconnect_to_map(int object_index, const location *location_in)
{
    object_header_datum *header =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index);
    object_datum *object = header->datum;

    if ( object->object.parent_object_index == -1 )
    {
        const location *loc = location_in;
        location local_location;
        if ( !location_in )
        {
            scenario_location_from_point(&local_location, &object->object.bounding_sphere_center);
            loc = &local_location;
            if ( (uint16_t)local_location.cluster_index == 0xFFFF )
                scenario_location_from_point(&local_location, &object->object.position);
        }

        if ( (uint16_t)loc->cluster_index == 0xFFFF )
        {
            object->object.flags |= (1u << _object_outside_of_map_bit);
        }
        else
        {
            object->object.location.leaf_index = loc->leaf_index;
            object->object.location.cluster_index = loc->cluster_index;
            object->object.location.bonus = loc->bonus;
            header->cluster_index = object->object.location.cluster_index;
            object->object.flags &= ~(1u << _object_outside_of_map_bit);
        }
        header->flags &= ~(1u << _object_header_child_bit);

        unsigned int object_flags = header->datum->object.flags;
        cluster_partition *partition = (object_flags & (1u << _object_has_collision_model_bit)) != 0
            ? &collideable_object_cluster_partition
            : &noncollideable_object_cluster_partition;
        /* arg6 corrected: disasm passes &object->object.location (r8=object+0x98, deref'd as
           location: cluster_partition_reconnect reads location.cluster_index at +4); the
           reconstruction wrongly reused the int object_flags here */
        cluster_partition_reconnect(partition, object_index,
            &object->object.first_cluster_reference_index,
            &object->object.bounding_sphere_center, object->object.bounding_sphere_radius,
            &object->object.location);

        if ( (header->flags & (1u << _object_header_automatically_deactivate_bit)) != 0 )
        {
            int cluster = (uint16_t)header->cluster_index;
            if ( cluster == 0xFFFF
                || !BIT_VECTOR_TEST_FLAG(players_get_combined_pvs(), (int16_t)cluster) )
            {
                if ( (object->object.flags & (1u << _object_deleted_when_deactivated_bit)) != 0 )
                    object_delete(object_index);
            }
            else
            {
                object_activate(object_index);
            }
        }
    }
    else
    {
        object_datum *parent = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum,
                                                  object->object.parent_object_index)->datum;
        header->datum->object.next_object_index = parent->object.first_child_object_index;
        parent->object.first_child_object_index = object_index;
        header->flags |= (1u << _object_header_child_bit);
        object->object.location.cluster_index = -1;
    }

    object->object.flags |= (1u << _object_connected_to_map_bit);
    header->flags |= (1u << _object_header_connected_to_map_bit);
}
