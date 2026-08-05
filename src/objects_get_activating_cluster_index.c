/* objects_get_activating_cluster_index @0x836ECA98 — return the structure-BSP cluster that is currently
 * driving PVS activation, or -1 if none. With a cluster activation source (type 2) the stored cluster is
 * returned directly; with an object activation source (type 1) the object's ultimate parent is resolved
 * and its cluster index returned (provided the object is valid and flagged as occupying a cluster).
 * A stale object source is cleared to inactive. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_globals.h"
#include "headers/object_datum.h"
#include "headers/object_flags.h"
#include "headers/object_type.h"
#include "headers/object_pvs_activation_type.h"
#include "headers/blam_data_globals.h"


extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern int object_get_ultimate_parent(int object_index);

int objects_get_activating_cluster_index(void)
{
    int activation_type = (uint16_t)object_globals->pvs_activation_type;
    if (activation_type != _pvs_activation_object)
    {
        if (activation_type == _pvs_activation_cluster)
            return (uint16_t)object_globals->pvs_activation.cluster_index;
        return -1;
    }

    if (!object_try_and_get_and_verify_type(object_globals->pvs_activation.object_index, object_mask_all))
    {
        object_globals->pvs_activation_type = 0;
        return -1;
    }

    int parent_index = (uint16_t)object_get_ultimate_parent(object_globals->pvs_activation.object_index);
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, parent_index)->datum;

    if ((object->object.flags & (1u << _object_connected_to_map_bit)) == 0)
        return -1;

    int cluster_index = (uint16_t)object->object.location.cluster_index;
    if (cluster_index == 0xFFFF)
        return -1;
    return cluster_index;
}
