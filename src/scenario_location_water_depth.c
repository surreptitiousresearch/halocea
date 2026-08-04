/* scenario_location_water_depth @0x83703990 — returns how deep `position` sits below the water surface
 * of its cluster's fog region, or a sentinel when there is no water. -FLT_MAX when the cluster/fog is
 * absent or the fog region isn't water; +FLT_MAX when the water region has no bounding plane (unbounded);
 * otherwise the negated signed distance below the fog plane plus the definition's water depth.
 *
 * DEVIATION: the decompiler emitted phantom uninitialized v6/v7 (the fog plane is only set on one
 * branch); reconstructed from disassembly. */

#include "headers/structure_bsp.h"
#include "headers/structure_cluster.h"
#include "headers/structure_fog_plane.h"
#include "headers/fog_definition.h"
#include "headers/fog_definition_flags.h"
#include "headers/fog_designator.h"
#include "headers/location.h"
#include "headers/real_point3d.h"
#include "headers/real_plane3d.h"
#include "headers/global_tag_instances.h"
#include <stdint.h>

/* attested callee: returns full-word tag index (lwz r3,0x2C / li r3,-1; callers cmpwi r3,-1 directly) */
extern int scenario_fog_region_get_fog_index(int16_t fog_region_index);

float scenario_location_water_depth(const location *location, const real_point3d *position)
{
    int cluster_index = location->cluster_index;
    if (cluster_index == -1)
        return -3.4028235e38f;

    structure_cluster *cluster =
        &((structure_cluster *)global_structure_bsp->clusters.address)[cluster_index];
    __int16 fog_ref = cluster->fog_designator;
    if (fog_ref == -1)
        return -3.4028235e38f;

    __int16 fog_region_index;
    real_plane3d *plane;   /* fog plane; null when the cluster has no bounding plane */
    if (FOG_DESIGNATOR_IS_PLANE(fog_ref))
    {
        /* high bit set: index into the fog_planes block */
        structure_fog_plane *fog_plane =
            &((structure_fog_plane *)global_structure_bsp->fog_planes.address)[FOG_DESIGNATOR_TO_INDEX(fog_ref)];
        plane = &fog_plane->plane;
        fog_region_index = fog_plane->region_index;
    }
    else
    {
        fog_region_index = FOG_DESIGNATOR_TO_INDEX(fog_ref);
        plane = 0;
    }

    int fog_index = scenario_fog_region_get_fog_index(fog_region_index);
    if (fog_index == -1)
        return -3.4028235e38f;

    fog_definition *fog = TAG_GET(fog_definition, fog_index);
    if ((fog->flags & (1u << _fog_definition_is_water_bit)) == 0)
        return -3.4028235e38f;   /* fog region is not water */

    if (!plane)
        return 3.4028235e38f;    /* water with no bounding plane: unbounded depth */

    float distance = (position->n[0] * plane->n.n[0]
                      + (position->n[2] * plane->n.n[2] + position->n[1] * plane->n.n[1])) - plane->d;
    return -(distance + fog->distance_to_water_plane);
}
