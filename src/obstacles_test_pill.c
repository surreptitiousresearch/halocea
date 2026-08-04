/* obstacles_test_pill @0x8382FE78 — cast a 2D "pill" (a ray thickened by `radius`) from `point` along
 * `direction` for up to `distance` against every obstacle disc, keeping whichever is hit nearest. Discs with
 * their flags bit0 set ("optional") are skipped when `ignore_optional` is set. Standard ray/circle
 * intersection: skip discs behind the ray (`dot(direction, disc.center-point) <= 0`), a disc the ray starts
 * inside (`combined_radius >= distance-to-center`) hits at distance 0, otherwise solve for the near
 * intersection distance. Returns whether anything was hit (`result->disc_index != -1`). */

#include <stdint.h>
#include "headers/obstacles.h"
#include "headers/real_point2d.h"
#include "headers/real_vector2d.h"
#include "headers/obstacles_test_pill_result.h"
#include "headers/disc_flags.h"

extern float __fsqrts(float x);

uint8_t obstacles_test_pill(const obstacles *obstacles, int16_t ignore_disc_index, const real_point2d *point,
        const real_vector2d *direction, float radius, float distance, uint8_t ignore_optional,
        obstacles_test_pill_result *result)
{
    result->distance = distance;
    result->disc_index = -1;
    result->obstacle_index = -1;

    for (int16_t disc_index = 0; disc_index < obstacles->disc_count; disc_index++)
    {
        if (disc_index == ignore_disc_index)
            continue;

        const disc *candidate = &obstacles->discs[disc_index];
        if (ignore_optional && (candidate->flags & (1u << _disc_optional_bit)) != 0)
            continue;

        float dx = candidate->center.n[0] - point->n[0];
        float dy = candidate->center.n[1] - point->n[1];
        float projected = direction->n[1] * dy + direction->n[0] * dx;

        if (projected <= 0.0f)
            continue;

        float combined_radius = candidate->radius + radius;
        float dist2_minus_radius2 = (dx * dx + dy * dy) - combined_radius * combined_radius;

        uint8_t hit;
        float hit_distance;

        if (dist2_minus_radius2 <= 0.0f)
        {
            hit = 1;
            hit_distance = 0.0f;
        }
        else
        {
            float discriminant = projected * projected - dist2_minus_radius2;
            if (discriminant < 0.0f)
            {
                hit = 0;
                hit_distance = 0.0f;
            }
            else
            {
                hit = 1;
                hit_distance = projected - __fsqrts(discriminant);
            }
        }

        if (hit && result->distance > hit_distance)
        {
            result->distance = hit_distance;
            result->disc_index = disc_index;
            result->obstacle_index = candidate->obstacle_index;
        }
    }

    return result->disc_index != -1;
}
