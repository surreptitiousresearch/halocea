/* actor_move_test_avoidance_vector @0x837C7F78 — cast one parameterized avoidance ray through the world and the
 * collected obstacle cylinders, returning the nearest collision. From the avoidance data's local frame
 * (origin/forward/left/up) and the ray's offset/divergence/length it builds a world ray_origin (origin + the
 * offset projected through the frame, scaled by avoid_width) and a world ray_direction (the divergence projected
 * through the frame, scaled by avoid_distance*length). It then tests: (1) the segment from origin to ray_origin
 * against the collision BSP — a hit means an immediate collision (t = 0); (2) otherwise the main ray against the
 * BSP — a hit records the surface t; (3) every avoidance cylinder via pill_test_vector3d, keeping the nearest.
 * collision_t receives the nearest t (FLT_MAX if clear). Returns 2 for a BSP hit, 1 for a cylinder-only hit, 0
 * for none. When collision_timer is supplied it is reset to 0 on any hit, else incremented (saturating at 255).
 *
 * DEVIATION: the ray frame math adds global_zero_vector3d ({0,0,0}) as a generic base; those +0 terms are
 * bit-exact no-ops and are dropped. FPR-shadow ABI traps corrupted the two call sites: disasm (0x837C8144 /
 * 0x837C81A8) shows collision_bsp_test_vector uses point/vector = ray_origin/ray_direction and one shared result
 * scratch (Hex-Rays punned that pointer through the low half of a fused slot); pill_test_vector3d's real args are
 * (base, height, width, ray_origin, ray_direction, &pill_t, &pill_normal) — the height/width floats burn the r4/r5
 * GPR shadow so Hex-Rays mislabeled point/vector/t/normal. Both BSP tests use maximum_t = 1.0. */

#include <stdint.h>
#include "headers/vector_avoidance_data.h"
#include "headers/vector_avoidance_ray.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/collision_bsp.h"
#include "headers/collision_bsp_test_vector_result.h"
#include "headers/actor_vector_avoidance_result.h"
#include "headers/blam_data_globals.h"


extern uint8_t collision_bsp_test_vector(unsigned int flags, const struct collision_bsp *bsp, int16_t breakable_surface_count, const uint8_t *breakable_surface_flags, const real_point3d *point, const real_vector3d *vector, float maximum_t, collision_bsp_test_vector_result *result);
extern uint8_t pill_test_vector3d(const real_point3d *base, float height, float width, const real_point3d *point, const real_vector3d *vector, float *t_reference, real_vector3d *normal);

int16_t actor_move_test_avoidance_vector(vector_avoidance_data *avoidance_data,
        const vector_avoidance_ray *avoidance_ray, real_point3d *ray_origin, real_vector3d *ray_direction,
        float *collision_t, uint8_t *collision_timer)
{
    *collision_t = 3.4028235e38f;
    int16_t hit_kind = _actor_vector_avoidance_clear;

    real_vector3d forward = avoidance_data->forward;
    real_vector3d left = avoidance_data->left;
    real_vector3d up = avoidance_data->up;
    real_vector3d offset = avoidance_ray->offset;  /* recovered: real_point3d -> real_vector3d (member retyped) */
    real_vector3d divergence = avoidance_ray->divergence;
    float avoid_width = avoidance_data->avoid_width;

    ray_origin->n[0] = (avoid_width * ((up.n[0] * offset.n[2])
                    + ((left.n[0] * offset.n[1]) + (forward.n[0] * offset.n[0]))))
            + avoidance_data->origin.n[0];
    ray_origin->n[1] = (avoid_width * ((up.n[1] * offset.n[2])
                    + ((left.n[1] * offset.n[1]) + (forward.n[1] * offset.n[0]))))
            + avoidance_data->origin.n[1];
    ray_origin->n[2] = ((((up.n[2] * offset.n[2])
                    + ((left.n[2] * offset.n[1]) + (forward.n[2] * offset.n[0]))) * avoid_width))
            + avoidance_data->origin.n[2];

    float ray_scale = (avoidance_data->avoid_distance * avoidance_ray->length);
    ray_direction->n[0] = (ray_scale * ((up.n[0] * divergence.n[2])
                    + ((left.n[0] * divergence.n[1]) + (forward.n[0] * divergence.n[0]))));
    ray_direction->n[1] = (((up.n[1] * divergence.n[2])
                    + ((left.n[1] * divergence.n[1]) + (forward.n[1] * divergence.n[0])))
            * ray_scale);
    ray_direction->n[2] = (((up.n[2] * divergence.n[2])
                    + ((left.n[2] * divergence.n[1]) + (forward.n[2] * divergence.n[0])))
            * ray_scale);

    real_vector3d origin_offset;
    origin_offset.n[0] = ray_origin->n[0] - avoidance_data->origin.n[0];
    origin_offset.n[1] = ray_origin->n[1] - avoidance_data->origin.n[1];
    origin_offset.n[2] = ray_origin->n[2] - avoidance_data->origin.n[2];

    collision_bsp_test_vector_result bsp_result;
    float bsp_t;
    if ( collision_bsp_test_vector(3, avoidance_data->bsp, 0, nullptr, &avoidance_data->origin, &origin_offset,
            1.0f, &bsp_result) )
    {
        bsp_t = 0.0f;
        *collision_t = bsp_t;
        hit_kind = _actor_vector_avoidance_obstructed_structure;
    }
    else if ( collision_bsp_test_vector(3, avoidance_data->bsp, 0, nullptr, ray_origin, ray_direction, 1.0f,
            &bsp_result) )
    {
        bsp_t = bsp_result.t;
        *collision_t = bsp_t;
        hit_kind = _actor_vector_avoidance_obstructed_structure;
    }

    for ( int i = 0; i < avoidance_data->avoidance_object_count; i = (int16_t)(i + 1) )
    {
        float pill_t;
        real_vector3d pill_normal;
        if ( pill_test_vector3d(&avoidance_data->avoidance_objects[i].base,
                avoidance_data->avoidance_objects[i].height, avoidance_data->avoidance_objects[i].width,
                ray_origin, ray_direction, &pill_t, &pill_normal)
          && pill_t < *collision_t )
        {
            *collision_t = pill_t;
            hit_kind = _actor_vector_avoidance_obstructed_object;
        }
    }

    if ( !collision_timer )
        return hit_kind;
    if ( hit_kind > 0 )
    {
        *collision_timer = 0;
        return hit_kind;
    }
    if ( *collision_timer != 255 )
        ++*collision_timer;
    return hit_kind;
}
