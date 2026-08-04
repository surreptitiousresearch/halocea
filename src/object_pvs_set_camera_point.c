/* object_pvs_set_camera_point @0x836EC9B8 — force the potentially-visible-set to activate from a scenario
 * cutscene camera point's cluster (activation type 2). Clears the override if the index is -1 or the point
 * is outside the BSP (no cluster). */

#include <stdint.h>
#include "headers/object_globals.h"
#include "headers/scenario.h"
#include "headers/scenario_cutscene_camera_point.h"
#include "headers/location.h"
#include "headers/real_point3d.h"
#include "headers/object_pvs_activation_type.h"
#include "headers/blam_data_globals.h"

#include "headers/real_point3d.h"
extern void scenario_location_from_point(location *location, const real_point3d *point);

void object_pvs_set_camera_point(int16_t camera_point_index)
{
    if ( camera_point_index == -1 )
    {
        object_globals->pvs_activation_type = _pvs_activation_normal;
        return;
    }

    location point_location;
    scenario_location_from_point(&point_location,
        &((scenario_cutscene_camera_point *)global_scenario->cutscene_camera_points.address)[camera_point_index].position);

    if ( (unsigned __int16)point_location.cluster_index == 0xFFFF )
    {
        object_globals->pvs_activation_type = _pvs_activation_normal;
    }
    else
    {
        object_globals->pvs_activation_type = _pvs_activation_cluster;
        object_globals->pvs_activation.cluster_index = point_location.cluster_index; /* recovered: *(__int16*)&pvs_activation -> .cluster_index */
    }
}
