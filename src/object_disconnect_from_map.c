/* object_disconnect_from_map @0x836EEEB8 — remove an object from the map's spatial structures. A parented
 * object is unlinked from its parent's child list (first_child_object_index / next_object_index chain).
 * A free object is disconnected from the appropriate cluster partition — collideable vs non-collideable per
 * object flags bit 0x2000000 — using its cluster-connection field, and deactivated if its header asks for
 * automatic deactivation. Finally the "connected to map" object flag (0x800) and the header's connected
 * bit (0x20) are cleared.
 *
 * DEVIATION: the decompiler's re-fetch of the header entry + active-bit clear inside the
 * automatically-deactivate branch is a verbatim compiler-inlined copy of object_deactivate @0x836EBC40
 * (already re-sourced; body matches exactly) — emitted as the call the source had. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_header_flags.h"
#include "headers/cluster_partition.h"
#include "headers/object_flags.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void cluster_partition_disconnect(cluster_partition *partition, int datum_index, int *first_cluster_reference);
extern void object_deactivate(int object_index);

void object_disconnect_from_map(int object_index)
{
    object_header_datum *header =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index);
    object_datum *object = header->datum;

    int parent_index = object->object.parent_object_index;
    if ( parent_index == -1 )
    {
        cluster_partition *partition;
        if ( (header->datum->object.flags & (1u << _object_has_collision_model_bit)) != 0 )
            partition = &collideable_object_cluster_partition;
        else
            partition = &noncollideable_object_cluster_partition;
        cluster_partition_disconnect(partition, object_index,
                                     &object->object.first_cluster_reference_index);

        if ( (header->flags & (1u << _object_header_automatically_deactivate_bit)) != 0 )
            object_deactivate(object_index);
    }
    else
    {
        object_datum *parent_object =
            object_try_and_get_and_verify_type(parent_index, object_mask_all);
        if ( parent_object && parent_object->object.first_child_object_index != -1 )
        {
            int *link = &parent_object->object.first_child_object_index;
            object_datum *sibling;
            int next_sibling;
            while ( 1 )
            {
                sibling = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, *link)->datum;
                next_sibling = sibling->object.next_object_index;
                if ( *link == object_index )
                    break;
                link = &sibling->object.next_object_index;
                if ( next_sibling == -1 )
                    goto done;
            }
            *link = next_sibling;
            sibling->object.next_object_index = -1;
        }
    }

done:
    object->object.flags &= ~(1u << _object_connected_to_map_bit);
    header->flags &= ~(1u << _object_header_connected_to_map_bit);
}
