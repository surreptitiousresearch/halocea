/* structure_visibility_build_surfaces_traverse_node @0x837C5BC0 — recursively descend the BSP3D node tree,
 * collecting visible surfaces within a bounding sphere/box/plane volume. At each node the quantized child AABB
 * is expanded, tested against the cull box and planes (skipped once the parent is fully inside), and the cull
 * state propagated to the children. The bounding sphere is then classified against the node's split plane to
 * decide which side(s) to descend; negative child indices are leaves (gathered via traverse_leaf), the rest
 * are interior nodes. Returns the running surface count.
 *
 * The database prototype is mangled by the float-argument GPR-shadow ABI (phantom args a12..a33). The real
 * 11-argument signature is the DB's leading 11 params. The traverse_leaf callee's DB prototype additionally
 * carries a phantom cull_sphere_center (a leaf doesn't need it); its corrected 9-argument form is below. */

#include <stdint.h>
#include "headers/structure_bsp.h"
#include "headers/collision_bsp.h"
#include "headers/bsp3d_node.h"
#include "headers/real_rectangle3d.h"
#include "headers/real_point3d.h"
#include "headers/real_plane3d.h"
#include "headers/byte_rectangle3d.h"
#include "headers/intersection.h"

extern void dequantize_byte_to_real_rectangle3d(const real_rectangle3d *parent, const byte_rectangle3d *compressed_rectangle, real_rectangle3d *result);
extern int16_t bounding_rectangles_intersect(const real_rectangle3d *test_rectangle, const real_rectangle3d *rectangle);
extern int16_t planes_intersect_rectangle(const real_rectangle3d *bounds, int16_t cull_plane_count, const real_plane3d *cull_planes);
extern int structure_visibility_build_surfaces_traverse_leaf(int leaf_index, const real_rectangle3d *parent_bounds, unsigned int *surface_flags, int *surface_indices, int16_t maximum_count, const real_rectangle3d *cull_bounds, int16_t cull_plane_count, const real_plane3d *cull_planes, int16_t intersection);

int16_t structure_visibility_build_surfaces_traverse_node(int node_index, const real_rectangle3d *parent_bounds, unsigned int *surface_flags, int *surface_indices, int16_t maximum_count, const real_point3d *cull_sphere_center, float cull_sphere_radius, const real_rectangle3d *cull_bounds, int16_t cull_plane_count, const real_plane3d *cull_planes, int16_t intersection)
{
    structure_bsp *bsp = global_structure_bsp;
    const collision_bsp *collision = (const collision_bsp *)bsp->collision_bsp.address;
    int found = 0;

    real_rectangle3d node_bounds;
    dequantize_byte_to_real_rectangle3d(parent_bounds,
        &((const byte_rectangle3d *)bsp->nodes.address)[node_index], &node_bounds);

    int16_t child_cull_plane_count = cull_plane_count;
    int16_t node_intersection = intersection;
    if (intersection != _intersection_in)
    {
        int16_t rectangle_hit = bounding_rectangles_intersect(&node_bounds, cull_bounds);
        int16_t plane_hit = rectangle_hit
            ? planes_intersect_rectangle(&node_bounds, cull_plane_count, cull_planes)
            : _intersection_out;
        if (plane_hit == _intersection_in)
            child_cull_plane_count = 0;   /* fully inside the planes — stop plane-culling children */
        node_intersection = (rectangle_hit > plane_hit) ? plane_hit : rectangle_hit;
    }

    if (node_intersection != 0)
    {
        const bsp3d_node *node = &((const bsp3d_node *)collision->bsp3d.nodes.address)[node_index];
        const real_plane3d *plane = &((const real_plane3d *)collision->bsp3d.planes.address)[node->plane_index];
        float distance = (cull_sphere_center->n[0] * plane->n.n[0]
                          + (plane->n.n[2] * cull_sphere_center->n[2] + plane->n.n[1] * cull_sphere_center->n[1]))
                         - plane->d;

        unsigned char descend_side[2];
        descend_side[0] = distance < cull_sphere_radius;
        descend_side[1] = distance > -cull_sphere_radius;

        for (int side = 0; side < 2; side = (int16_t)(side + 1))
        {
            if (!descend_side[side])
                continue;

            int child = node->child_indices[side];
            if (child < 0)
            {
                if (child != -1)
                    found = (int16_t)(structure_visibility_build_surfaces_traverse_leaf(child, &node_bounds,
                        surface_flags, &surface_indices[(int16_t)found], maximum_count - found, cull_bounds,
                        child_cull_plane_count, cull_planes, node_intersection) + found);
            }
            else
            {
                found = (int16_t)(structure_visibility_build_surfaces_traverse_node(child, &node_bounds,
                    surface_flags, &surface_indices[(int16_t)found], maximum_count - found, cull_sphere_center,
                    cull_sphere_radius, cull_bounds, child_cull_plane_count, cull_planes, node_intersection)
                    + found);
            }
        }
    }

    return found;
}
