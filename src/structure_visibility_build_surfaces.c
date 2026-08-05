/* structure_visibility_build_surfaces @0x837C60D8 — gather the BSP surfaces overlapping a bounding sphere
 * into surface_indices, using a per-surface bit vector to dedup. Dispatches one of three ways:
 *   - small spheres (radius below structure_sphere_threshold) walk the whole BSP3D node tree;
 *   - when an explicit cluster index list is supplied, walk just those clusters;
 *   - otherwise locate the sphere-center cluster and, if valid, walk the clusters it touches (else fall back
 *     to the full node-tree walk).
 * When no bounding box is supplied, one is derived from the sphere's AABB.
 *
 * The decompiler badly mangled this function via the float-argument GPR-shadow ABI (inventing phantom args
 * a10..a29 and mistyping the bit vector as real_plane3d[]); the argument flow below is recovered from the
 * disassembly. NOTE: the database prototype for structure_visibility_build_surfaces_traverse_clusters is
 * wrong — it lists a spurious bounding_sphere_center; the real call passes no center (see prototype below). */

#include <stdint.h>
#include "headers/structure_bsp.h"
#include "headers/real_point3d.h"
#include "headers/real_rectangle3d.h"
#include "headers/real_plane3d.h"
#include "headers/location.h"
#include <string.h>
#include "headers/blam_data_globals.h"
#include "headers/bit_vector.h"

#define STRUCTURE_SURFACE_FLAGS_WORDS 4096   /* bit vector, one bit per BSP surface */


extern int16_t structure_visibility_build_surfaces_traverse_node(int node_index, const real_rectangle3d *parent_bounds, unsigned int *surface_flags, int *surface_indices, int16_t maximum_count, const real_point3d *cull_sphere_center, float cull_sphere_radius, const real_rectangle3d *cull_bounds, int16_t cull_plane_count, const real_plane3d *cull_planes, int16_t intersection);

/* Corrected prototype (DB lists a phantom bounding_sphere_center): no center argument. */
extern int structure_visibility_build_surfaces_traverse_clusters(int *surface_indices, int16_t maximum_count, const real_rectangle3d *bounding_box, int16_t bounding_surface_count, const real_plane3d *bounding_surfaces, unsigned int *surface_flags, int16_t cluster_count, int16_t *cluster_indices, float bounding_sphere_radius);

extern void scenario_location_from_point(location *location, const real_point3d *point);
extern int16_t structure_clusters_in_sphere(int16_t cluster_index, const real_point3d *position, float radius, int16_t maximum_count, int16_t *intersected_indices);

int16_t structure_visibility_build_surfaces(int *surface_indices, int16_t maximum_count, const real_point3d *bounding_sphere_center, float bounding_sphere_radius, const real_rectangle3d *bounding_box, int16_t bounding_surface_count, const real_plane3d *bounding_surfaces, int16_t cluster_count, int16_t *cluster_indices)
{
    structure_bsp *bsp = global_structure_bsp;
    unsigned int surface_flags[STRUCTURE_SURFACE_FLAGS_WORDS];

    memset(surface_flags, 0, 4 * BIT_VECTOR_SIZE_IN_LONGS(bsp->surfaces.count));

    real_rectangle3d sphere_bounds;
    if (!bounding_box)
    {
        sphere_bounds.n[0] = bounding_sphere_center->n[0] - bounding_sphere_radius;
        sphere_bounds.n[1] = bounding_sphere_center->n[0] + bounding_sphere_radius;
        sphere_bounds.n[2] = bounding_sphere_center->n[1] - bounding_sphere_radius;
        sphere_bounds.n[3] = bounding_sphere_center->n[1] + bounding_sphere_radius;
        sphere_bounds.n[4] = bounding_sphere_center->n[2] - bounding_sphere_radius;
        sphere_bounds.n[5] = bounding_sphere_center->n[2] + bounding_sphere_radius;
        bounding_box = &sphere_bounds;
    }

    if (bounding_sphere_radius < structure_sphere_threshold)
        return structure_visibility_build_surfaces_traverse_node(0, &bsp->world_bounds, surface_flags,
            surface_indices, maximum_count, bounding_sphere_center, bounding_sphere_radius, bounding_box,
            bounding_surface_count, bounding_surfaces, 1);

    if (cluster_indices)
        return structure_visibility_build_surfaces_traverse_clusters(surface_indices, maximum_count,
            bounding_box, bounding_surface_count, bounding_surfaces, surface_flags,
            cluster_count, cluster_indices, bounding_sphere_radius);  /* arg order corrected 2026-07-31: float radius last (f1) */

    location center_location;
    scenario_location_from_point(&center_location, bounding_sphere_center);
    if ((uint16_t)center_location.cluster_index == 0xFFFF)
        return structure_visibility_build_surfaces_traverse_node(0, &bsp->world_bounds, surface_flags,
            surface_indices, maximum_count, bounding_sphere_center, bounding_sphere_radius, bounding_box,
            bounding_surface_count, bounding_surfaces, 1);

    int16_t found_cluster_indices[512];
    int16_t found_cluster_count = structure_clusters_in_sphere(center_location.cluster_index,
        bounding_sphere_center, bounding_sphere_radius, 512, found_cluster_indices);
    return structure_visibility_build_surfaces_traverse_clusters(surface_indices, maximum_count, bounding_box,
        bounding_surface_count, bounding_surfaces, surface_flags, found_cluster_count,
        found_cluster_indices, bounding_sphere_radius);  /* arg order corrected 2026-07-31: float radius last (f1) */
}
