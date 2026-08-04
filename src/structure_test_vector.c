/* structure_test_vector @0x837448E8 — cast a ray (point + vector) against the structure BSP and find
 * the first render surface with a valid lightmap that it strikes. Returns 1 and fills the lightmap /
 * material / surface indices, barycentric (s,t) and the collision point on success; 0 on miss.
 *
 * If a struck surface has no resolvable render surface or no lightmap (index 0xFFFF), and the hit was
 * through a penetrable surface (collision_result.flags bit 0), the search nudges the start point a
 * small step along the ray and retries; otherwise it stops. */

#include <stdint.h>
#include "headers/structure_bsp.h"
#include "headers/structure_lightmap.h"
#include "headers/collision_result.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/collision_test_flags.h"

extern uint8_t collision_test_vector(unsigned int flags, const real_point3d *point, const real_vector3d *vector, int ignore_object_index, collision_result *collision);
extern uint8_t structure_render_surface_from_point_and_leaf(const real_point3d *point, int leaf_index, int plane_index, int16_t *lightmap_index, int16_t *material_index, int *surface_index, float *s, float *t);

uint8_t structure_test_vector(const real_point3d *p, const real_vector3d *v, real_point3d *collision_point,
        int16_t *lightmap_index, int16_t *material_index, int *surface_index, float *s, float *t)
{
    int found = 0;
    int done;
    collision_result collision;

    *collision_point = *p;
    do
    {
        done = 1;
        if ( collision_test_vector((1u << _collision_test_front_facing_surfaces_bit)
                                       | (1u << _collision_test_structure_bit),
                                   collision_point, v, -1, &collision) )
        {
            structure_bsp *structure = global_structure_bsp;
            int plane_designator = collision.plane_designator;

            *collision_point = collision.point;

            if ( structure_render_surface_from_point_and_leaf(collision_point,
                        collision.location.leaf_index, plane_designator & 0x7FFFFFFF,
                        lightmap_index, material_index, surface_index, s, t)
              && (unsigned __int16)((structure_lightmap *)structure->lightmaps.address)[*lightmap_index].bitmap_index != 0xFFFF )
            {
                found = 1;
            }
            else if ( (collision.flags & 1) != 0 )
            {
                /* penetrable surface: advance the start point a small step and keep searching */
                done = 0;
                collision_point->n[0] = (v->n[0] * (float)0.00024414062) + collision_point->n[0];
                collision_point->n[1] = (v->n[1] * (float)0.00024414062) + collision_point->n[1];
                collision_point->n[2] = (v->n[2] * (float)0.00024414062) + collision_point->n[2];
            }
        }
    }
    while ( !done );

    return found;
}
