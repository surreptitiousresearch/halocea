/* unit_clip_to_aiming_bounds @ 0x836C9968 — clamp a world-space aiming (or looking) direction to the
 * unit's allowed aiming cone. Builds the unit's orientation frame (forward = row0, up = row2, left = row0
 * x row2), transforms the vector into that local frame, converts to yaw/pitch euler angles, clamps them
 * to the unit's screen bounds (yaw min/max, pitch min/max: unit.animation.aiming_screen_bounds for aiming,
 * looking_screen_bounds for looking), and if anything was clamped rebuilds the world vector. Returns 1 if
 * the vector was clamped. The bounds are skipped entirely when the relevant screen is disabled
 * (animation.aiming/looking_with_euler_screen). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/real_vector3d.h"
#include "headers/real_euler_angles2d.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_point3d.h"
#include "headers/blam_data_globals.h"


extern void object_get_orientation(int object_index, real_vector3d *forward, real_vector3d *up);
extern real_vector3d *matrix4x3_inverse_transform_normal(const real_matrix4x3 *matrix, const real_vector3d *normal, real_vector3d *result);
extern real_euler_angles2d *euler_angles2d_from_vector3d(real_euler_angles2d *angles, const real_vector3d *vector);
extern real_vector3d *vector3d_from_euler_angles2d(real_vector3d *vector, const real_euler_angles2d *angles);
extern real_vector3d *matrix4x3_transform_normal(const real_matrix4x3 *matrix, const real_vector3d *normal, real_vector3d *result);

int unit_clip_to_aiming_bounds(int unit_index, real_vector3d *vector, uint8_t use_aiming_screen)
{
    int clamped = 0;
    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);

    char screen_enabled = use_aiming_screen ? unit->unit.animation.aiming_with_euler_screen : unit->unit.animation.looking_with_euler_screen;
    float *bounds = use_aiming_screen ? (float *)&unit->unit.animation.aiming_screen_bounds : (float *)&unit->unit.animation.looking_screen_bounds;

    if ( screen_enabled )
    {
        real_matrix4x3 frame;
        frame.scale = 1.0f;
        /* matrix rows (float[3]) received as vector3d out-params: row0 = forward, row2 = up. */
        object_get_orientation(unit_index, (real_vector3d *)&frame.n[0], (real_vector3d *)&frame.n[2]);
        /* left = forward x up */
        frame.n[1][0] = frame.n[0][2] * frame.n[2][1] - frame.n[2][2] * frame.n[0][1];
        frame.n[1][1] = frame.n[0][0] * frame.n[2][2] - frame.n[0][2] * frame.n[2][0];
        frame.n[1][2] = frame.n[2][0] * frame.n[0][1] - frame.n[0][0] * frame.n[2][1];
        frame.n[3][0] = global_origin3d->n[0];
        frame.n[3][1] = global_origin3d->n[1];
        frame.n[3][2] = global_origin3d->n[2];

        real_vector3d local_vector;
        matrix4x3_inverse_transform_normal(&frame, vector, &local_vector);
        real_euler_angles2d angles;
        euler_angles2d_from_vector3d(&angles, &local_vector);

        if ( angles.n[0] < bounds[0] )
        {
            angles.n[0] = bounds[0];
            clamped = 1;
        }
        else if ( angles.n[0] > bounds[1] )
        {
            angles.n[0] = bounds[1];
            clamped = 1;
        }

        if ( angles.n[1] < bounds[2] )
        {
            angles.n[1] = bounds[2];
            clamped = 1;
        }
        else if ( angles.n[1] > bounds[3] )
        {
            angles.n[1] = bounds[3];
            clamped = 1;
        }

        if ( (unsigned char)clamped )
        {
            vector3d_from_euler_angles2d(&local_vector, &angles);
            matrix4x3_transform_normal(&frame, &local_vector, vector);
        }
    }

    return clamped;
}
