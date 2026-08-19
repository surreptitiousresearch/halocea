/* structure_visibility_build_surfaces_traverse_clusters @0x837C54C8 — gather, into surface_indices, the
 * visible BSP surfaces that fall inside a bounding volume, scanning only a supplied list of clusters. For each
 * cluster's subclusters whose AABB intersects the bounding box and the optional cull planes, each listed
 * surface that is both flagged visible (render.environment_surface_flags) and not yet collected
 * (surface_flags) is appended and marked. Returns the count, stopping at maximum_count.
 *
 * The database prototype is mangled: it invents a phantom bounding_sphere_center at r5 and places the float
 * bounding_sphere_radius mid-list. Disasm proves neither: the callee passes r5 (mr r22,r5) as
 * bounding_rectangles_intersect's `test_rectangle` (so r5 = bounding_box, NOT a sphere centre) and r6
 * (mr r18,r6) as planes_intersect_rectangle's `cull_plane_count` (so r6 = bounding_surface_count). The
 * float bounding_sphere_radius is the FINAL argument (f1, unused; its skipped GPR slot is on the stack),
 * which is why every GPR r3..r10 is a genuine integer/pointer argument. cluster_count is int16 (extsh r9).
 * Signature corrected 2026-07-31 (float moved to last; earlier reconstruction had it 4th, shifting every
 * argument after bounding_box by one register). */

#include <stdint.h>
#include "headers/structure_bsp.h"
#include "headers/structure_cluster.h"
#include "headers/structure_subcluster.h"
#include "headers/render_globals.h"
#include "headers/real_rectangle3d.h"
#include "headers/real_plane3d.h"

extern int16_t bounding_rectangles_intersect(const real_rectangle3d *test_rectangle, const real_rectangle3d *rectangle);
extern int16_t planes_intersect_rectangle(const real_rectangle3d *bounds, int16_t cull_plane_count, const real_plane3d *cull_planes);

int structure_visibility_build_surfaces_traverse_clusters(int *surface_indices, int16_t maximum_count, const real_rectangle3d *bounding_box, int16_t bounding_surface_count, const real_plane3d *bounding_surfaces, unsigned int *surface_flags, int16_t cluster_count, int16_t *cluster_indices, float bounding_sphere_radius)
{
    (void)bounding_sphere_radius;   /* final ABI arg (f1), unused by this traversal */
    int found = 0;
    structure_bsp *bsp = global_structure_bsp;

    for (int ci = 0; ci < cluster_count; ci = (int16_t)(ci + 1))
    {
        if ((int16_t)found >= maximum_count)
            break;

        structure_cluster *cluster = (structure_cluster *)bsp->clusters.address + cluster_indices[ci];
        int subcluster_count = cluster->subclusters.count;

        for (int sci = 0; sci < subcluster_count; sci = (int16_t)(sci + 1))
        {
            if ((int16_t)found >= maximum_count)
                break;

            structure_subcluster *subcluster = (structure_subcluster *)cluster->subclusters.address + sci;
            if (!bounding_rectangles_intersect(bounding_box, &subcluster->world_bounds))
                continue;
            if (!planes_intersect_rectangle(&subcluster->world_bounds, bounding_surface_count,
                                            bounding_surfaces))
                continue;

            int *surface_list = (int *)subcluster->surface_indices.address;
            int surface_count = subcluster->surface_indices.count;
            for (int k = 0; k < surface_count; ++k)
            {
                int surface_index = surface_list[k];
                int word = surface_index >> 5;
                unsigned int bit = 1 << (surface_index & 0x1F);
                if ((render.environment_surface_flags[word] & bit) != 0 && (surface_flags[word] & bit) == 0)
                {
                    if ((int16_t)found >= maximum_count)
                        break;
                    surface_flags[word] |= bit;
                    surface_indices[found] = surface_index;
                    found = (int16_t)(found + 1);
                }
            }
        }
    }

    return found;
}
