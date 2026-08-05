/* observer_check_penetration @0x8370FCE8 — pull the third-person camera in when the line from the focus point to
 * the desired camera position (focus - forward*(distance+safe_distance)) is obstructed by world geometry, so the
 * camera does not clip through walls. On entry *distance is the desired pull-back distance; on exit it is scaled
 * by the fraction of that distance that is actually clear.
 *
 * Algorithm: (1) test the straight-back ray and record its clear fraction; (2) probe four perturbed camera
 * positions offset by +-up and +-cross (cross = up x forward), each scaled by distance*sine_region_angle, and
 * keep whichever probe is most obstructed (smallest clear fraction); (3) if a probe hit, run a 10-step bisection
 * along that winning offset direction to localize the obstruction boundary; (4) blend the straight-back and
 * best-probe clear fractions by the bisection parameter and scale *distance by the result.
 *
 * DEVIATION: safe_distance is the trailing float arg (f1) and does not shadow any pointer slot, so the signature
 * is unaffected. The decompiler's __int64/fcfid scratch shuffles for the probe sign (+-1) and the (upper>=0)
 * boolean are restored as plain casts; the probe address arithmetic `&v50 + (i&1) + ((2*i)&2)` is expressed as
 * indexing perturbation[i & 1] with sign (i & 2 ? +1 : -1). */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/location.h"

extern const float sine_region_angle;

extern void scenario_location_from_point(location *location, const real_point3d *point);
extern uint8_t scenario_location_underwater(const location *location, const real_point3d *position, int16_t *optional_weather_palette_index);
extern uint8_t observer_collision_test_with_t(const real_point3d *p0, const real_point3d *p1, float *t, uint8_t ignore_media);

void observer_check_penetration(real_point3d *focus_position, const real_vector3d *forward,
                                const real_vector3d *up, float *distance, float safe_distance)
{
    float collision_t = 1.0f;

    location focus_location;
    scenario_location_from_point(&focus_location, focus_position);
    uint8_t ignore_media = scenario_location_underwater(&focus_location, focus_position, nullptr);

    /* desired camera position straight back from the focus point */
    float reach = *distance + safe_distance;
    real_point3d desired_camera;
    desired_camera.n[0] = focus_position->n[0] - forward->n[0] * reach;
    desired_camera.n[1] = focus_position->n[1] - forward->n[1] * reach;
    desired_camera.n[2] = focus_position->n[2] - forward->n[2] * reach;
    observer_collision_test_with_t(focus_position, &desired_camera, &collision_t, ignore_media);

    /* build the +up and +cross perturbation offsets, each scaled by distance*sine_region_angle */
    float step = *distance * sine_region_angle;
    real_vector3d perturbation[2];
    perturbation[0].n[0] = up->n[0] * step;
    perturbation[0].n[1] = up->n[1] * step;
    perturbation[0].n[2] = up->n[2] * step;
    perturbation[1].n[0] = (up->n[1] * forward->n[2] - up->n[2] * forward->n[1]) * step;  /* cross.x */
    perturbation[1].n[1] = (up->n[2] * forward->n[0] - up->n[0] * forward->n[2]) * step;  /* cross.y */
    perturbation[1].n[2] = (up->n[0] * forward->n[1] - up->n[1] * forward->n[0]) * step;  /* cross.z */

    float first_t = collision_t;
    float best_t = first_t;
    real_vector3d *best_offset = nullptr;
    float best_sign = 0.0f;

    for (int i = 0; i < 4; i = (int16_t)(i + 1))
    {
        float sign = (i & 2) ? 1.0f : -1.0f;
        real_vector3d *offset = &perturbation[i & 1];
        real_point3d probe;
        probe.n[0] = sign * offset->n[0] + desired_camera.n[0];
        probe.n[1] = offset->n[1] * sign + desired_camera.n[1];
        probe.n[2] = offset->n[2] * sign + desired_camera.n[2];
        if (observer_collision_test_with_t(focus_position, &probe, &collision_t, ignore_media)
            && collision_t < best_t)
        {
            best_t = collision_t;
            best_offset = offset;
            best_sign = sign;
        }
    }

    float distance_scale = first_t;

    if (best_offset)
    {
        float offset_x = best_offset->n[0];
        float offset_y = best_offset->n[1];
        float offset_z = best_offset->n[2];
        float last_miss_t = first_t;
        float last_hit_t = best_t;
        float lower = 0.0f;
        float upper = best_sign;

        for (int j = 0; j < 10; j = (int16_t)(j + 1))
        {
            float mid = (upper + lower) * 0.5f;
            real_point3d probe;
            probe.n[0] = offset_x * mid + desired_camera.n[0];
            probe.n[1] = offset_y * mid + desired_camera.n[1];
            probe.n[2] = offset_z * mid + desired_camera.n[2];
            uint8_t hit = observer_collision_test_with_t(focus_position, &probe, &collision_t, ignore_media);
            if (hit && __fabs(collision_t - last_hit_t) < 0.1f)
            {
                upper = mid;
                last_hit_t = collision_t;
            }
            else
            {
                lower = mid;
                last_miss_t = hit ? collision_t : 1.0f;
            }
        }

        float selector;
        if (last_miss_t < last_hit_t)
            selector = lower;
        else
            selector = (upper >= 0.0f) ? 1.0f : 0.0f;

        if (last_miss_t < last_hit_t)
            upper = lower;
        if (selector == 0.0f)
            upper = -upper;

        distance_scale = (1.0f - upper) * best_t + upper * first_t;
    }

    *distance = *distance * distance_scale;
}
