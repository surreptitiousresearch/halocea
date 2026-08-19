/* structure_render_preprocess @0x837C6828 — per-frame BSP render setup: build the dynamic triangle list for
 * the visible environment surfaces, record whether the resulting geometry is valid, render any debug leaf /
 * portal overlays requested by the debug globals, and reset the fog offset. */

#include <stdint.h>
#include "headers/structure_render_globals.h"
#include "headers/render_globals.h"
#include "headers/structure_bsp.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"

extern int structure_render_build_dynamic_triangles(int *surface_indices, int16_t surface_count, unsigned int *surface_flags);
extern void render_debug_leaf_portals(const leaf_map *map, int leaf_index);
extern void render_debug_leaf_portal(const leaf_map *map, int portal_designator);


void structure_render_preprocess(void)
{
    structure_bsp *bsp = global_structure_bsp;

    int triangles_index = structure_render_build_dynamic_triangles(
        render.environment_surface_indices, render.environment_surface_count,
        render.environment_surface_flags);
    structure_render_globals.environment_geometry_valid = triangles_index != -1;
    structure_render_globals.environment_triangles_index = triangles_index;

    if (debug_leaf_index >= 0 && debug_leaf_index < bsp->leaf_map.leaves.count)
        render_debug_leaf_portals(&bsp->leaf_map, debug_leaf_index);

    if (debug_leaf_portal_index >= 0 && debug_leaf_portal_index < bsp->leaf_map.portals.count)
        render_debug_leaf_portal(&bsp->leaf_map, debug_leaf_portal_index);

    if (debug_leaf_portals)
    {
        for (int i = 0; i < bsp->leaf_map.portals.count; ++i)
            render_debug_leaf_portal(&bsp->leaf_map, i);
    }

    structure_render_globals.fog_offset_valid = 0;
    structure_render_globals.fog_offset_vector = *global_zero_vector3d;
}
