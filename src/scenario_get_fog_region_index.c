/* scenario_get_fog_region_index @ 0x83704708 — the fog region containing a point, or -1. The point's
 * cluster names a fog plane index; a "simple" cluster (high bit clear) returns the region directly,
 * while a "complex" cluster (high bit set) references a fog plane: the point must lie below that plane
 * (adjusted by the fog tag's depth) to be inside the region. The region index is the word at the head
 * of the fog plane.
 *
 * Cluster (104 bytes): +2 (word) fog plane index (bit15 = complex). Fog plane (32 bytes): +0 region
 * index, +4 normal, +16 plane distance. Fog tag: dword0 bit0 = enabled, +116 = depth.
 *
 * Deviation: the decompiler dropped the assignments of the fog-plane pointer, the region word and the
 * position pointer; recovered from the disassembly (r7 = fog plane, r6 = region word, r4 = position). */

#include "headers/structure_bsp.h"
#include "headers/structure_cluster.h"
#include "headers/structure_fog_plane.h"
#include "headers/fog_definition.h"
#include "headers/fog_definition_flags.h"
#include "headers/fog_designator.h"
#include "headers/location.h"
#include "headers/real_point3d.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"
#include <stdint.h>

/* attested callee: returns full-word tag index (lwz r3,0x2C / li r3,-1; callers cmpwi r3,-1 directly) */
extern int scenario_fog_region_get_fog_index(int16_t fog_region_index);

/* attested: int16_t return — all 3 callers extsh r11,r3 after the call */
int16_t scenario_get_fog_region_index(const location *location, const real_point3d *position)
{
    int cluster_index = location->cluster_index;
    int16_t cluster_fog;
    structure_fog_plane *fog_plane;
    int16_t region_index;
    int fog_index; /* full-word callee return, compared to -1 without extsh */
    float depth;

    if ( cluster_index == -1 )
        return -1;

    cluster_fog = ((structure_cluster *)global_structure_bsp->clusters.address)[cluster_index].fog_designator;
    if ( cluster_fog == -1 )
        return -1;

    if ( !FOG_DESIGNATOR_IS_PLANE(((structure_cluster *)global_structure_bsp->clusters.address)[cluster_index].fog_designator) )
        return FOG_DESIGNATOR_TO_INDEX(cluster_fog);  /* simple: region index directly */

    /* complex: (32*d) & 0xFFFE0 == (d & 0x7FFF)*32 — strip the 0x8000 designator flag, index the 32-byte fog planes */
    fog_plane = &((structure_fog_plane *)global_structure_bsp->fog_planes.address)[cluster_fog & 0x7FFF];
    region_index = fog_plane->region_index;
    fog_index = scenario_fog_region_get_fog_index(region_index);

    depth = 0.0f;
    if ( fog_index != -1 )
    {
        fog_definition *fog_tag = TAG_GET(fog_definition, fog_index);
        if ( (fog_tag->flags & (1u << _fog_definition_is_water_bit)) != 0 )
            depth = fog_tag->distance_to_water_plane;
    }

    /* inside the region when below the (depth-adjusted) fog plane */
    if ( !position
      || ((fog_plane->plane.n.n[0] * position->n[0]
           + (fog_plane->plane.n.n[2] * position->n[2] + fog_plane->plane.n.n[1] * position->n[1]))
          - fog_plane->plane.d) + depth < 0.0f )
    {
        return region_index;
    }
    return -1;
}
