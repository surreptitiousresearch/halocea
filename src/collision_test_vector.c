/* collision_test_vector @ 0x83773188 — cast a ray (point + vector) against the world and report the
 * nearest hit in `collision`. Depending on `flags` it tests the structure BSP surfaces, the active
 * fog plane of the impact cluster, and/or the collideable objects in the traversed clusters, then
 * resolves the BSP location of the impact point (nudging it off the surface so it lands in a leaf).
 *
 * flags bits: 0x20 = test structure surfaces, 0x40 = test fog planes, 0x80 = test objects,
 * 0x100000 = resolve a valid impact location. Bits 0..4 and 8..19 select the surface/object filter
 * masks passed down to collision_bsp_test_vector / object_test_vector.
 *
 * Deviations: the decompiler split the on-stack collision_bsp_test_vector_result into scalar locals
 * (t, plane, surface_index, plane_designator, flags, breakable_surface_index, material_index,
 * leaf_count, leaf_indices[]) and lost the address-of that it passes as the result pointer; it is
 * restored here as a single `bsp_result`. The fog plane scratch (n.x/n.y/n.z/d) and the cluster
 * object iterator are likewise restored to a real_plane3d and an int[4]. Cluster/leaf/material/fog
 * tag payloads are read by raw offset, matching the decompiler. */

#include <stdint.h>
#include "headers/structure_bsp.h"
#include "headers/structure_leaf.h"
#include "headers/structure_cluster.h"
#include "headers/structure_collision_material.h"
#include "headers/structure_fog_region.h"
#include "headers/structure_fog_palette_entry.h"
#include "headers/structure_fog_plane.h"
#include "headers/fog_definition.h"
#include "headers/collision_result.h"
#include "headers/collision_bsp_test_vector_result.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/location.h"
#include "headers/global_tag_instances.h"


#include "headers/data_array.h"
#include "headers/collision_result_type.h"
#include "headers/collision_test_flags.h"
#include "headers/collision_bsp_test_flags.h"
#include "headers/blam_data_globals.h"

extern double __fabs(double x);
extern const uint8_t *breakable_surface_flags_get(void);
extern uint8_t collision_bsp_test_vector(unsigned int flags, const struct collision_bsp *bsp, int16_t breakable_surface_count, const uint8_t *breakable_surface_flags, const real_point3d *point, const real_vector3d *vector, float maximum_t, collision_bsp_test_vector_result *result);
extern void structure_cluster_marker_begin(void);
extern void object_marker_begin(void);
extern uint8_t structure_cluster_mark(int16_t cluster_index);
extern int cluster_get_first_collideable_object(int *reference_index, int16_t cluster_index);
extern int cluster_get_next_collideable_object(int *reference_index);
extern uint8_t object_test_vector(int object_index, unsigned int flags, unsigned int bsp_flags, const real_point3d *point, const real_vector3d *vector, int ignore_object_index, collision_result *collision);
extern void object_marker_end(void);
extern void structure_cluster_marker_end(void);
extern int scenario_leaf_index_from_point(const real_point3d *point);
extern void scenario_location_from_point(location *location, const real_point3d *point);

extern int16_t get_material_type(int object_index, int16_t material_index);
uint8_t collision_test_vector(unsigned int flags, const real_point3d *point, const real_vector3d *vector,
                              int ignore_object_index, collision_result *collision)
{
    int leaf_loop_index = 0;
    unsigned int effective_flags = flags;
    int hit = 0;
    char test_structure = 0;
    location *impact_location = &collision->location;
    collision_bsp_test_vector_result bsp_result;

    if ( (flags & (1u << _collision_test_structure_bit)) != 0
      || (flags & (1u << _collision_test_media_bit)) != 0
      || (flags & (1u << _collision_test_objects_bit)) != 0 )
        test_structure = 1;

    collision->type = -1;
    collision->start_location.leaf_index = -1;
    collision->start_location.cluster_index = -1;
    collision->t = 1.0f;
    collision->location.leaf_index = -1;
    collision->location.cluster_index = -1;

    if ( !test_structure )
    {
        collision->t = 1.0f;
        collision->point.n[0] = point->n[0] + vector->n[0];
        collision->point.n[1] = point->n[1] + vector->n[1];
        collision->point.n[2] = point->n[2] + vector->n[2];
        scenario_location_from_point(&collision->location, &collision->point);
        return hit;
    }

    {
        unsigned int test_objects = (flags >> _collision_test_objects_bit) & 1;
        structure_bsp *bsp = global_structure_bsp;
        unsigned int bsp_flags;
        int leaf_count;

        /* default to testing both front- and back-facing surfaces, then mirror the requested
         * filter bits into the collision-bsp flag layout */
        if ( (flags & (1u << _collision_test_front_facing_surfaces_bit)) == 0 && (flags & (1u << _collision_test_back_facing_surfaces_bit)) == 0 )
            effective_flags = flags | 3;

        bsp_flags = (effective_flags & (1u << _collision_test_back_facing_surfaces_bit)) != 0
                        ? (effective_flags & (1u << _collision_test_front_facing_surfaces_bit)) | (1u << _collision_bsp_test_back_facing_surfaces_bit)
                        : effective_flags & (1u << _collision_test_front_facing_surfaces_bit);
        bsp_flags = (effective_flags & (1u << _collision_test_ignore_two_sided_surfaces_bit)) != 0
                        ? bsp_flags | (1u << _collision_bsp_test_ignore_two_sided_surfaces_bit)
                        : bsp_flags & ~(1u << _collision_bsp_test_ignore_two_sided_surfaces_bit);
        bsp_flags = (effective_flags & (1u << _collision_test_ignore_invisible_surfaces_bit)) != 0
                        ? bsp_flags | (1u << _collision_bsp_test_ignore_invisible_surfaces_bit)
                        : bsp_flags & ~(1u << _collision_bsp_test_ignore_invisible_surfaces_bit);
        bsp_flags = (effective_flags & (1u << _collision_test_ignore_breakable_surfaces_bit)) != 0
                        ? bsp_flags | (1u << _collision_bsp_test_ignore_breakable_surfaces_bit)
                        : bsp_flags & ~(1u << _collision_bsp_test_ignore_breakable_surfaces_bit);

        if ( collision_bsp_test_vector(bsp_flags, global_collision_bsp, 256, breakable_surface_flags_get(),
                                       point, vector, 3.4028235e38f, &bsp_result)
          && (effective_flags & (1u << _collision_test_structure_bit)) != 0 )
        {
            const real_plane3d *plane = bsp_result.plane;
            int plane_designator = bsp_result.plane_designator;
            int16_t material_type;

            collision->type = collision_result_structure;
            collision->t = bsp_result.t;
            collision->plane.normal.n[0] = plane->normal.n[0];
            collision->plane.normal.n[1] = plane->normal.n[1];
            collision->plane.normal.n[2] = plane->normal.n[2];
            collision->plane.d = plane->d;
            if ( plane_designator < 0 )
            {
                collision->plane.normal.n[0] = -collision->plane.normal.n[0];
                collision->plane.normal.n[1] = -collision->plane.normal.n[1];
                collision->plane.normal.n[2] = -collision->plane.normal.n[2];
                collision->plane.d = -collision->plane.d;
            }

            /* DEVIATION: donor get_material_type@0x837BE090's object_index==-1 branch was inlined here (object_index folded to -1, the current structure BSP's own collision_materials lookup); collapsed to a call. */
            material_type = get_material_type(-1, bsp_result.material_index);

            collision->plane_designator = plane_designator;
            collision->material_type = material_type;
            collision->surface_index = bsp_result.surface_index;
            collision->flags = bsp_result.flags;
            collision->breakable_surface_index = bsp_result.breakable_surface_index;
            collision->material_index = bsp_result.material_index;
            hit = 1;
        }

        leaf_count = bsp_result.leaf_count;
        if ( bsp_result.leaf_count > 0 )
        {
            int first_leaf = bsp_result.leaf_indices[0];
            int16_t start_cluster;
            int last_leaf;
            int16_t end_cluster;

            collision->start_location.leaf_index = first_leaf;
            if ( first_leaf == -1 )
                start_cluster = -1;
            else /* leaf -> cluster */
                start_cluster = ((structure_leaf *)global_structure_bsp->leaves.address)[first_leaf].cluster_index;
            collision->start_location.cluster_index = start_cluster;

            last_leaf = bsp_result.leaf_indices[leaf_count - 1];
            impact_location->leaf_index = last_leaf;
            if ( last_leaf == -1 )
                end_cluster = -1;
            else
                end_cluster = ((structure_leaf *)global_structure_bsp->leaves.address)[last_leaf].cluster_index;
            collision->location.cluster_index = end_cluster;
        }

        /* fog plane test: intersect the ray with the impact cluster's fog plane if it is closer */
        if ( (effective_flags & (1u << _collision_test_media_bit)) != 0 )
        {
            int16_t cluster_index = collision->location.cluster_index;
            if ( cluster_index != -1 )
            {
                int16_t fog_plane_index = ((structure_cluster *)bsp->clusters.address)[cluster_index].fog_designator;
                if ( fog_plane_index != -1
                  && (((structure_cluster *)bsp->clusters.address)[cluster_index].fog_designator & 0x8000) != 0 )
                {
                    structure_fog_plane *fog_plane =
                        &((structure_fog_plane *)bsp->fog_planes.address)[fog_plane_index];
                    if ( (uint16_t)fog_plane->runtime_material_type != 0xFFFF )
                    {
                        float nx = fog_plane->plane.normal.n[0];
                        float ny = fog_plane->plane.normal.n[1];
                        float nz = fog_plane->plane.normal.n[2];
                        float plane_d = fog_plane->plane.d;
                        int16_t fog_region = fog_plane->region_index;
                        /* lower the plane height by the region's fog-palette base offset */
                        int palette_index = ((structure_fog_region *)bsp->fog_regions.address)[fog_region].fog_palette_index;
                        fog_definition *fog_tag =
                            TAG_GET(fog_definition, ((structure_fog_palette_entry *)bsp->fog_palette.address)[palette_index].fog.index);
                        float adjusted_d = plane_d - fog_tag->distance_to_water_plane;

                        float vector_dot = vector->n[0] * nx + (vector->n[1] * ny + vector->n[2] * nz);
                        float point_dist = (point->n[0] * nx + (point->n[1] * ny + point->n[2] * nz)) - adjusted_d;

                        if ( (point_dist > 0.0f) != (vector_dot > 0.0f)
                          && __fabs(point_dist) < __fabs(vector_dot)
                          && __fabs(vector_dot) >= 0.000099999997f )
                        {
                            float fog_t = -(point_dist / vector_dot);
                            if ( collision->t > fog_t )
                            {
                                int16_t fog_material;
                                collision->t = fog_t;
                                collision->type = collision_result_media;
                                collision->plane.normal.n[0] = nx;
                                collision->plane.normal.n[1] = ny;
                                collision->plane.normal.n[2] = nz;
                                collision->plane.d = adjusted_d;
                                if ( point_dist >= 0.0f )
                                {
                                    fog_material = fog_plane->runtime_material_type;
                                }
                                else
                                {
                                    collision->plane.normal.n[0] = -collision->plane.normal.n[0];
                                    collision->plane.normal.n[1] = -collision->plane.normal.n[1];
                                    collision->plane.normal.n[2] = -collision->plane.normal.n[2];
                                    collision->plane.d = -collision->plane.d;
                                    fog_material = 28;
                                }
                                hit = 1;
                                collision->material_type = fog_material;
                            }
                        }
                    }
                }
            }
        }

        /* object test: walk the collideable objects in each traversed leaf's cluster */
        if ( (unsigned char)test_objects && leaf_count > 0 )
        {
            if ( (effective_flags & _collision_test_objects_all_types_flags) == 0 )
                effective_flags |= (unsigned int)_collision_test_objects_all_types_flags;
            structure_cluster_marker_begin();
            object_marker_begin();
            if ( bsp_result.leaf_count > 0 )
            {
                int *leaf_ptr = bsp_result.leaf_indices;
                do
                {
                    int16_t cluster;
                    if ( *leaf_ptr == -1 )
                        cluster = -1;
                    else
                        cluster = ((structure_leaf *)global_structure_bsp->leaves.address)[*leaf_ptr].cluster_index;
                    if ( structure_cluster_mark(cluster) )
                    {
                        int iterator[4];
                        int object_index;
                        for ( object_index = cluster_get_first_collideable_object(iterator, cluster);
                              object_index != -1;
                              object_index = cluster_get_next_collideable_object(iterator) )
                        {
                            object_datum *object =
                                DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
                            if ( object->object.magic_number != global_object_marker )
                            {
                                object->object.magic_number = global_object_marker;
                                if ( object_test_vector(object_index, effective_flags, bsp_flags,
                                                        point, vector, ignore_object_index, collision) )
                                    hit = 1;
                            }
                        }
                    }
                    ++leaf_loop_index;
                    ++leaf_ptr;
                }
                while ( leaf_loop_index < bsp_result.leaf_count );
            }
            object_marker_end();
            structure_cluster_marker_end();
        }

        if ( !(unsigned char)hit )
            collision->t = 1.0f;

        {
            float t = collision->t;
            collision->point.n[0] = (vector->n[0] * collision->t) + point->n[0];
            collision->point.n[1] = (vector->n[1] * t) + point->n[1];
            collision->point.n[2] = (vector->n[2] * t) + point->n[2];
        }

        if ( (effective_flags & (1u << _collision_test_try_to_keep_location_valid_bit)) != 0 && (unsigned char)hit
          && impact_location->leaf_index != -1
          && scenario_leaf_index_from_point(&collision->point) != impact_location->leaf_index )
        {
            /* the impact point landed in a different leaf than expected: push it off the surface
             * along the plane normal and re-resolve, backing along the ray if still out of bounds */
            collision->point.n[0] = (collision->plane.normal.n[0] * 0.00024414062f) + collision->point.n[0];
            collision->point.n[1] = (collision->plane.normal.n[1] * 0.00024414062f) + collision->point.n[1];
            collision->point.n[2] = (collision->plane.normal.n[2] * 0.00024414062f) + collision->point.n[2];
            scenario_location_from_point(&collision->location, &collision->point);
            if ( impact_location->leaf_index == -1 )
            {
                float normal_dot = (collision->plane.normal.n[0] * vector->n[0])
                                 + ((collision->plane.normal.n[2] * vector->n[2])
                                    + (collision->plane.normal.n[1] * vector->n[1]));
                float step = (normal_dot == 0.0f) ? 0.03125f : (float)(0.000244140625 / __fabs(normal_dot));
                float t = collision->t;
                while ( 1 )
                {
                    float new_t = t - step;
                    if ( new_t <= 0.0f )
                        new_t = 0.0f;
                    collision->t = new_t;
                    collision->point.n[0] = (vector->n[0] * new_t) + point->n[0];
                    collision->point.n[1] = (vector->n[1] * new_t) + point->n[1];
                    collision->point.n[2] = (vector->n[2] * new_t) + point->n[2];
                    scenario_location_from_point(&collision->location, &collision->point);
                    t = collision->t;
                    if ( t <= 0.0f )
                        break;
                    if ( impact_location->leaf_index != -1 )
                        return hit;
                }
            }
        }
    }
    return hit;
}
