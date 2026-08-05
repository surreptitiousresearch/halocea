/* structure_get_planar_fog_definition_index @0x83? — resolve the planar fog tag definition affecting a cluster.
 * With get_fog_from_sky set, the sky's indoor fog plane reference is returned directly. Otherwise the cluster's
 * fog plane reference is followed: a referenced fog plane (high bit set) names a fog region indirectly, a direct
 * reference uses the low bits; the fog region in turn names a fog palette entry whose fog tag index is returned.
 * Returns -1 when the cluster has no fog, the chain is broken, or no sky exists. */

#include <stdint.h>
#include "headers/structure_bsp.h"
#include "headers/structure_cluster.h"
#include "headers/structure_fog_plane.h"
#include "headers/structure_fog_region.h"
#include "headers/structure_fog_palette_entry.h"
#include "headers/fog_designator.h"
#include "headers/sky.h"

extern sky *scenario_get_sky(int16_t sky_index);

int structure_get_planar_fog_definition_index(const structure_bsp *structure, int16_t cluster_index, uint8_t get_fog_from_sky)
{
    if (cluster_index == -1)
        return -1;

    if (get_fog_from_sky)
    {
        sky *sky_tag = scenario_get_sky(0);
        if (sky_tag)
            return sky_tag->indoor_fog_plane.index;
        return -1;
    }

    int16_t fog_plane_reference = ((structure_cluster *)structure->clusters.address)[cluster_index].fog_designator;
    if (fog_plane_reference == -1)
        return -1;

    int16_t fog_region_index =
        FOG_DESIGNATOR_IS_PLANE(fog_plane_reference)
            /* (32*d) & 0xFFFE0 == (d & 0x7FFF)*32 — strip the 0x8000 flag; region_index @0 in the 32B plane */
            ? ((const structure_fog_plane *)structure->fog_planes.address)[fog_plane_reference & 0x7FFF].region_index
            : FOG_DESIGNATOR_TO_INDEX(fog_plane_reference);
    if (fog_region_index == -1)
        return -1;

    int fog_palette_index = ((structure_fog_region *)structure->fog_regions.address)[fog_region_index].fog_palette_index;
    if (fog_palette_index == -1)
        return -1;

    return ((structure_fog_palette_entry *)structure->fog_palette.address)[fog_palette_index].fog.index;
}
