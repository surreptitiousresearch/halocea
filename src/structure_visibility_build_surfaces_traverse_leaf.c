/* structure_visibility_build_surfaces_traverse_leaf @0x837C5678 — gather the visible surfaces of one BSP3D
 * leaf into surface_indices. Expands the leaf's quantized AABB, tests it against the cull box and planes
 * (unless the parent is already fully inside), then walks the leaf's surface-reference span: each referenced
 * surface that is both flagged visible (render.environment_surface_flags) and not yet collected
 * (surface_flags) is appended and marked, stopping at maximum_count. Returns the running count.
 *
 * The database prototype is float-shadow-mangled (phantom args a12..a29) and carries a phantom
 * cull_sphere_center; the corrected 9-argument signature below matches the call site in traverse_node. */

#include <stdint.h>
#include "headers/structure_bsp.h"
#include "headers/structure_leaf.h"
#include "headers/structure_surface_reference.h"
#include "headers/render_globals.h"
#include "headers/real_rectangle3d.h"
#include "headers/real_plane3d.h"
#include "headers/byte_rectangle3d.h"

#include "headers/byte_rectangle3d.h"
extern void dequantize_byte_to_real_rectangle3d(const real_rectangle3d *parent, const byte_rectangle3d *compressed_rectangle, real_rectangle3d *result);
extern int16_t bounding_rectangles_intersect(const real_rectangle3d *test_rectangle, const real_rectangle3d *rectangle);
extern int16_t planes_intersect_rectangle(const real_rectangle3d *bounds, int16_t cull_plane_count, const real_plane3d *cull_planes);

int structure_visibility_build_surfaces_traverse_leaf(int leaf_index, const real_rectangle3d *parent_bounds, unsigned int *surface_flags, int *surface_indices, int16_t maximum_count, const real_rectangle3d *cull_bounds, int16_t cull_plane_count, const real_plane3d *cull_planes, int16_t intersection)
{
    int found = 0;
    structure_bsp *bsp = global_structure_bsp;
    structure_leaf *leaf = (structure_leaf *)bsp->leaves.address + leaf_index;

    real_rectangle3d leaf_bounds;
    dequantize_byte_to_real_rectangle3d(parent_bounds, &leaf->bounds, &leaf_bounds);

    __int16 leaf_intersection = intersection;
    if (intersection != 2)
    {
        __int16 rectangle_hit = bounding_rectangles_intersect(&leaf_bounds, cull_bounds);
        __int16 plane_hit = planes_intersect_rectangle(&leaf_bounds, cull_plane_count, cull_planes);
        leaf_intersection = (rectangle_hit > plane_hit) ? plane_hit : rectangle_hit;
    }

    if (leaf_intersection)
    {
        int first_reference = leaf->first_surface_reference_index;
        __int16 surface_count = leaf->surface_reference_count;
        for (int reference = first_reference; reference < surface_count + first_reference; ++reference)
        {
            int surface_index =
                ((const structure_surface_reference *)bsp->surface_references.address)[reference].surface_index;
            int word = surface_index >> 5;
            unsigned int bit = 1 << (surface_index & 0x1F);
            if ((render.environment_surface_flags[word] & bit) != 0 && (surface_flags[word] & bit) == 0)
            {
                if ((__int16)found >= maximum_count)
                    return found;
                surface_flags[word] |= bit;
                surface_indices[found] = surface_index;
                found = (__int16)(found + 1);
            }
        }
    }

    return found;
}
