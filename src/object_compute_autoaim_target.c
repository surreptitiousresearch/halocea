/* object_compute_autoaim_target @0x836C5A44 — compute the autoaim "stick" point for a target object given
 * an aim ray (position + direction). The target's autoaim volume is a capsule (pill): a segment from
 * `base` along `height`, with radius `width`. This finds the point on that segment closest to the aim
 * ray, then pulls it perpendicularly toward the ray so it lies at most `width` from the aim line. The
 * result is the point the reticle should be magnetized to.
 *
 * The clamp parameter t locates the closest point on the segment to the aim line (line-line closest
 * approach), clamped to [0,1]; when the aim direction is parallel to the pill axis the cross product
 * degenerates and the base point is used directly. */

#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
extern float __fsqrts(float);

extern void biped_get_autoaim_pill(int biped_index, real_point3d *base, real_vector3d *height, float *width);

void object_compute_autoaim_target(int object_index, const real_point3d *position,
                                   const real_vector3d *direction, real_point3d *target)
{
    real_point3d  pill_base;
    real_vector3d pill_axis;
    float         pill_width[2];
    biped_get_autoaim_pill(object_index, &pill_base, &pill_axis, pill_width);
    float pill_radius = pill_width[0];

    /* cross = direction x pill_axis, indexed (x,y,z) = (cross_x, cross_y, cross_z). */
    float cross_z = direction->n[1] * pill_axis.n[0] - direction->n[0] * pill_axis.n[1];
    float cross_y = direction->n[0] * pill_axis.n[2] - direction->n[2] * pill_axis.n[0];
    float cross_x = direction->n[2] * pill_axis.n[1] - direction->n[1] * pill_axis.n[2];
    float cross_length_sq = cross_x * cross_x + (cross_z * cross_z + cross_y * cross_y);

    if (cross_length_sq <= 0.0f)
    {
        /* Aim direction parallel to the pill axis: snap to the base point. */
        target->n[0] = pill_base.n[0];
        target->n[1] = pill_base.n[1];
        target->n[2] = pill_base.n[2];
    }
    else
    {
        /* Parameter of the closest point on the pill segment to the aim line. */
        float to_base_x = position->n[0] - pill_base.n[0];
        float to_base_y = position->n[1] - pill_base.n[1];
        float to_base_z = position->n[2] - pill_base.n[2];
        float numerator =
            (direction->n[2] * to_base_y - direction->n[1] * to_base_z) * cross_x
          + ((direction->n[1] * to_base_x - direction->n[0] * to_base_y) * cross_z
          +  (direction->n[0] * to_base_z - direction->n[2] * to_base_x) * cross_y);
        float t = numerator / cross_length_sq;

        float clamped_t = 0.0f;
        if (t >= 0.0f)
        {
            clamped_t = 1.0f;
            if (t <= 1.0f)
                clamped_t = t;
        }

        target->n[0] = clamped_t * pill_axis.n[0] + pill_base.n[0];
        target->n[1] = clamped_t * pill_axis.n[1] + pill_base.n[1];
        target->n[2] = clamped_t * pill_axis.n[2] + pill_base.n[2];
    }

    /* Pull the closest point perpendicularly toward the aim ray, by at most the pill radius. */
    float prev_target_y = target->n[1];
    float prev_target_z = target->n[2];

    float delta_x = target->n[0] - position->n[0];
    float delta_y = target->n[1] - position->n[1];
    float delta_z = target->n[2] - position->n[2];
    float along = -(direction->n[0] * delta_x + (direction->n[1] * delta_y + direction->n[2] * delta_z));

    float perp_x = direction->n[0] * along + delta_x;
    float perp_y = direction->n[1] * along + delta_y;
    float perp_z = direction->n[2] * along + delta_z;
    float perp_length_sq = perp_x * perp_x + (perp_z * perp_z + perp_y * perp_y);

    if (perp_length_sq > pill_radius * pill_radius)
    {
        float scale = pill_radius / __fsqrts(perp_length_sq);
        perp_x = perp_x * scale;
        perp_y = perp_y * scale;
        perp_z = perp_z * scale;
    }

    target->n[0] = target->n[0] - perp_x;
    target->n[1] = prev_target_y - perp_y;
    target->n[2] = prev_target_z - perp_z;
}
