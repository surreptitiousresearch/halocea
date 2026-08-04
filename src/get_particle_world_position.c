/* get_particle_world_position @0x8380BB00 — computes a glow particle's world position for the current
 * frame. The glow's per-marker object-space frames (position + up vector, and a reconstructed side vector
 * = forward x up) are interpolated through a non-uniform cubic spline evaluated at the particle's
 * parameter t, giving a base position plus two orthogonal basis vectors. The particle is then swung around
 * that base by its (t-driven) angle at its radial distance_to_object.
 *
 * The general path (>= 4 markers) builds a 4-marker window around the active interval. The 2- and
 * 3-marker paths are degenerate special cases that build the four spline control points from the single
 * (or double) available segment. Those degenerate paths carry faithful shipped quirks and are reproduced
 * verbatim: (a) several z-component interior control points are lerped onto the *y* base of the endpoint
 * rather than the z base; (b) the side-vector control points (side_ctrl) are never filled on the 2/3
 * marker paths; and (c) the 3-marker interval-1 path reads knot1 from the never-written knot scratch.
 * These only bite when a glow has fewer than four markers. */

#include "headers/glow_datum.h"
#include "headers/glow_particle.h"
#include "headers/object_marker.h"
#include "headers/real_vector3d.h"
#include "headers/real_point3d.h"

extern void nonuniform_cubic_spline_vector3d(real_vector3d *result, const real_vector3d *f0, const real_vector3d *f1, const real_vector3d *f2, const real_vector3d *f3, float t0, float t1, float t2, float t3, float t);
extern double sin(double x);
extern double cos(double x);

void get_particle_world_position(glow_datum *glow, glow_particle *particle, float rotation_per_unit_t)
{
    real_vector3d position_ctrl[4]; /* spline control points -> particle->position */
    real_vector3d up_ctrl[4];       /* spline control points -> up basis */
    real_vector3d side_ctrl[4];     /* spline control points -> side basis (forward x up) */
    real_vector3d up_spline_result;
    real_vector3d side_spline_result;
    float knot_scratch[4]; /* knot times gathered on the general path (v101/v102 slot in the original) */
    float knot0, knot1, knot2, knot3;
    int number_of_markers;
    int marker_index;
    float t;

    /* locate the interval [k, k+1) of marker_time_index[] that contains the particle's t */
    marker_index = 0;
    {
        int last = glow->number_of_markers - 1;
        if (last > 0)
        {
            t = particle->t;
            int i = 0;
            do
            {
                if (glow->marker_time_index[i] <= t && glow->marker_time_index[i + 1] > t)
                    break;
                marker_index = (__int16)(i + 1);
                i = marker_index;
            } while (marker_index < last);
        }
        if (marker_index < 0)
            marker_index = 0;
        else if (marker_index > last)
            marker_index = glow->number_of_markers - 1;
    }
    particle->parent_marker_index = (__int16)marker_index;

    number_of_markers = (unsigned __int16)glow->number_of_markers;

    if (number_of_markers == 2)
    {
        /* single segment: endpoints marker 0 / marker 1, interior control points at t=0.25 / 0.75 */
        float m0_pos_y = glow->markers[0].matrix.position.n[1];
        float seg = glow->marker_time_index[1] - glow->marker_time_index[0];
        float m0_pos_x = glow->markers[0].matrix.position.n[0];
        float m1_pos_x = glow->markers[1].matrix.position.n[0];
        float m1_up_x = glow->markers[1].matrix.up.n[0];
        float m1_up_y = glow->markers[1].matrix.up.n[1];
        float m0_up_x = glow->markers[0].matrix.up.n[0];
        float m0_up_z = glow->markers[0].matrix.up.n[2];
        float m0_up_y = glow->markers[0].matrix.up.n[1];
        float m1_pos_z = glow->markers[1].matrix.position.n[2];
        float m1_up_z = glow->markers[1].matrix.up.n[2];

        knot0 = glow->marker_time_index[0];
        knot3 = glow->marker_time_index[1];
        knot1 = seg * 0.25f + glow->marker_time_index[0];
        knot2 = seg * 0.75f + glow->marker_time_index[0];

        position_ctrl[0].n[0] = m0_pos_x;
        position_ctrl[0].n[1] = m0_pos_y;
        position_ctrl[0].n[2] = glow->markers[0].matrix.position.n[2];
        position_ctrl[3].n[0] = m1_pos_x;
        position_ctrl[3].n[1] = glow->markers[1].matrix.position.n[1];
        position_ctrl[3].n[2] = m1_pos_z;

        up_ctrl[0].n[0] = m0_up_x;
        up_ctrl[0].n[1] = m0_up_y;
        up_ctrl[0].n[2] = m0_up_z;
        up_ctrl[3].n[0] = m1_up_x;
        up_ctrl[3].n[1] = m1_up_y;
        up_ctrl[3].n[2] = m1_up_z;

        /* QUIRK: z interior control points lerp onto the y base (m0_pos_y / m0_up_y), matching shipped code */
        position_ctrl[1].n[2] = (m1_pos_z - position_ctrl[0].n[2]) * 0.25f + m0_pos_y;
        position_ctrl[2].n[2] = (m1_pos_z - position_ctrl[0].n[2]) * 0.75f + m0_pos_y;
        position_ctrl[1].n[0] = (m1_pos_x - m0_pos_x) * 0.25f + m0_pos_x;
        position_ctrl[1].n[1] = (position_ctrl[3].n[1] - m0_pos_y) * 0.25f + m0_pos_y;
        position_ctrl[2].n[1] = (position_ctrl[3].n[1] - m0_pos_y) * 0.75f + m0_pos_y;
        position_ctrl[2].n[0] = (m1_pos_x - m0_pos_x) * 0.75f + m0_pos_x;
        up_ctrl[1].n[1] = (m1_up_y - m0_up_y) * 0.25f + m0_up_y;
        up_ctrl[1].n[2] = (m1_up_z - up_ctrl[0].n[2]) * 0.25f + m0_up_y; /* QUIRK: z onto y base */
        up_ctrl[1].n[0] = (m1_up_x - m0_up_x) * 0.25f + m0_up_x;
        up_ctrl[2].n[1] = (m1_up_y - m0_up_y) * 0.75f + m0_up_y;
        up_ctrl[2].n[0] = (m1_up_x - m0_up_x) * 0.75f + m0_up_x;
        up_ctrl[2].n[2] = (m1_up_z - up_ctrl[0].n[2]) * 0.75f + m0_up_y; /* QUIRK: z onto y base */
        goto evaluate_splines;
    }

    if (number_of_markers == 3)
    {
        /* endpoints marker 0 / marker 2; marker 1 supplies the interior control points */
        float m0_pos_y = glow->markers[0].matrix.position.n[1];
        float m0_pos_z = glow->markers[0].matrix.position.n[2];
        float m2_pos_x = glow->markers[2].matrix.position.n[0];
        int interval_is_first = (marker_index == 0);
        int interval_is_second = (marker_index == 1);
        float m2_pos_y = glow->markers[2].matrix.position.n[1];
        float m2_pos_z = glow->markers[2].matrix.position.n[2];
        float m0_up_x = glow->markers[0].matrix.up.n[0];
        float m0_up_y = glow->markers[0].matrix.up.n[1];
        float m0_up_z = glow->markers[0].matrix.up.n[2];
        float m2_up_x = glow->markers[2].matrix.up.n[0];
        float m2_up_y = glow->markers[2].matrix.up.n[1];
        float m2_up_z = glow->markers[2].matrix.up.n[2];

        knot0 = glow->marker_time_index[0];
        knot3 = glow->marker_time_index[2];

        position_ctrl[0].n[0] = glow->markers[0].matrix.position.n[0];
        position_ctrl[0].n[1] = m0_pos_y;
        position_ctrl[0].n[2] = m0_pos_z;
        position_ctrl[3].n[0] = m2_pos_x;
        position_ctrl[3].n[1] = m2_pos_y;
        position_ctrl[3].n[2] = m2_pos_z;
        up_ctrl[0].n[0] = m0_up_x;
        up_ctrl[0].n[1] = m0_up_y;
        up_ctrl[0].n[2] = m0_up_z;
        up_ctrl[3].n[0] = m2_up_x;
        up_ctrl[3].n[1] = m2_up_y;
        up_ctrl[3].n[2] = m2_up_z;

        if (interval_is_first)
        {
            /* interval [0,1]: marker 1 = control point 1, midpoint(marker1,marker2) = control point 2 */
            float m1_pos_z = glow->markers[1].matrix.position.n[2];
            float m1_pos_x = glow->markers[1].matrix.position.n[0];
            float m1_up_x = glow->markers[1].matrix.up.n[0];
            float m1_up_y = glow->markers[1].matrix.up.n[1];

            knot1 = glow->marker_time_index[1];

            position_ctrl[1].n[1] = glow->markers[1].matrix.position.n[1];
            position_ctrl[1].n[2] = m1_pos_z;
            position_ctrl[1].n[0] = m1_pos_x;
            up_ctrl[1].n[0] = m1_up_x;
            up_ctrl[1].n[1] = m1_up_y;
            up_ctrl[1].n[2] = glow->markers[1].matrix.up.n[2];

            up_ctrl[2].n[2] = (up_ctrl[3].n[2] - up_ctrl[1].n[2]) * 0.5f + m1_up_y; /* QUIRK: z onto y base */
            knot2 = (knot3 - knot1) * 0.5f + knot1;
            position_ctrl[2].n[1] = (position_ctrl[3].n[1] - position_ctrl[1].n[1]) * 0.5f + position_ctrl[1].n[1];
            position_ctrl[2].n[2] = (position_ctrl[3].n[2] - m1_pos_z) * 0.5f + position_ctrl[1].n[1]; /* QUIRK: z onto y base */
            position_ctrl[2].n[0] = (position_ctrl[3].n[0] - m1_pos_x) * 0.5f + m1_pos_x;
            up_ctrl[2].n[0] = (up_ctrl[3].n[0] - m1_up_x) * 0.5f + m1_up_x;
            up_ctrl[2].n[1] = (up_ctrl[3].n[1] - m1_up_y) * 0.5f + m1_up_y;
            goto evaluate_splines;
        }
        if (interval_is_second)
        {
            /* interval [1,2]: midpoint(marker0,marker1) = control point 1, marker 1 = control point 2 */
            float m1_pos_y = glow->markers[1].matrix.position.n[1];
            float m1_up_y = glow->markers[1].matrix.up.n[1];
            float m1_pos_x = glow->markers[1].matrix.position.n[0];
            float m1_up_z = glow->markers[1].matrix.up.n[2];

            up_ctrl[2].n[0] = glow->markers[1].matrix.up.n[0];
            position_ctrl[2].n[1] = m1_pos_y;
            up_ctrl[2].n[1] = m1_up_y;
            position_ctrl[2].n[0] = m1_pos_x;
            up_ctrl[2].n[2] = m1_up_z;
            position_ctrl[2].n[2] = glow->markers[1].matrix.position.n[2];

            knot2 = (glow->marker_time_index[1] - knot0) * 0.5f + knot0;
            position_ctrl[1].n[2] = (position_ctrl[2].n[2] - position_ctrl[0].n[2]) * 0.5f + position_ctrl[0].n[1]; /* QUIRK: z onto y base */
            position_ctrl[1].n[1] = (m1_pos_y - position_ctrl[0].n[1]) * 0.5f + position_ctrl[0].n[1];
            up_ctrl[1].n[0] = (up_ctrl[2].n[0] - up_ctrl[0].n[0]) * 0.5f + up_ctrl[0].n[0];
            up_ctrl[1].n[1] = (m1_up_y - up_ctrl[0].n[1]) * 0.5f + up_ctrl[0].n[1];
            position_ctrl[1].n[0] = (m1_pos_x - position_ctrl[0].n[0]) * 0.5f + position_ctrl[0].n[0];
            up_ctrl[1].n[2] = (m1_up_z - up_ctrl[0].n[2]) * 0.5f + up_ctrl[0].n[1]; /* QUIRK: z onto y base */
            goto label_31;
        }
        /* interval index 2 (t past the end) falls through with control points 1/2 unset — shipped edge */
    }
    else
    {
        /* general path: build a 4-marker window around the active interval */
        int center = 0;
        int last = (__int16)number_of_markers - 1;
        if (last > 0)
        {
            float tt = particle->t;
            int i = 0;
            do
            {
                if (glow->marker_time_index[i] <= tt && tt <= glow->marker_time_index[i + 1])
                    break;
                center = (__int16)(i + 1);
                i = center;
            } while (center < last);
        }
        if (center < 0)
            center = 0;
        else if (center > last)
            center = last;

        /* widen [left, right] outward until it spans four knots (clamped at the marker ends) */
        int left = center;
        int right = (__int16)(center + 1);
        if ((right - center) + 1 < 4)
        {
            do
            {
                if (left > 0)
                    left = left - 1;
                if (right < last)
                    right = (__int16)(right + 1);
            } while ((right - left) + 1 < 4);
        }

        int base = left;
        for (int i = 0; i < 4; i++)
        {
            object_marker *m = &glow->markers[glow->marker_order[i + base]];
            position_ctrl[i].n[0] = m->matrix.position.n[0];
            position_ctrl[i].n[1] = m->matrix.position.n[1];
            position_ctrl[i].n[2] = m->matrix.position.n[2];
            up_ctrl[i].n[0] = m->matrix.up.n[0];
            up_ctrl[i].n[1] = m->matrix.up.n[1];
            up_ctrl[i].n[2] = m->matrix.up.n[2];
            /* side basis = forward x up (reconstructed per marker) */
            side_ctrl[i].n[0] = m->matrix.forward.n[2] * m->matrix.up.n[1] - m->matrix.forward.n[1] * m->matrix.up.n[2];
            side_ctrl[i].n[1] = m->matrix.forward.n[0] * m->matrix.up.n[2] - m->matrix.forward.n[2] * m->matrix.up.n[0];
            side_ctrl[i].n[2] = m->matrix.forward.n[1] * m->matrix.up.n[0] - m->matrix.forward.n[0] * m->matrix.up.n[1];
            knot_scratch[i] = glow->marker_time_index[i + base];
        }
        knot3 = knot_scratch[3];
        knot0 = knot_scratch[0];
    }

    knot2 = knot_scratch[2];
label_31:
    knot1 = knot_scratch[1]; /* QUIRK: unset for the 3-marker interval-1 path; reproduces its stale read */
evaluate_splines:
    nonuniform_cubic_spline_vector3d((real_vector3d *)&particle->position, &position_ctrl[0], &position_ctrl[1],
                                     &position_ctrl[2], &position_ctrl[3], knot0, knot1, knot2, knot3,
                                     particle->t);
    nonuniform_cubic_spline_vector3d(&up_spline_result, &up_ctrl[0], &up_ctrl[1], &up_ctrl[2], &up_ctrl[3],
                                     knot0, knot1, knot2, knot3, particle->t);
    nonuniform_cubic_spline_vector3d(&side_spline_result, &side_ctrl[0], &side_ctrl[1], &side_ctrl[2],
                                     &side_ctrl[3], knot0, knot1, knot2, knot3, particle->t);

    {
        float angle = particle->t * rotation_per_unit_t + particle->initial_angle;
        float sin_a = (float)sin(angle);
        float cos_a = (float)cos(angle);
        float distance = particle->distance_to_object;
        real_point3d base_pos = particle->position; /* spline-1 result before we overwrite it */

        particle->position.n[0] = (side_spline_result.n[0] * cos_a + up_spline_result.n[0] * sin_a) * distance
                                  + base_pos.n[0];
        particle->position.n[1] = (side_spline_result.n[1] * cos_a + up_spline_result.n[1] * sin_a) * distance
                                  + base_pos.n[1];
        particle->position.n[2] = (side_spline_result.n[2] * cos_a + up_spline_result.n[2] * sin_a) * distance
                                  + base_pos.n[2];
    }
}
