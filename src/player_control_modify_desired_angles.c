/* player_control_modify_desired_angles @ 0x836DEA20 — apply this frame's look deltas to a player's
 * desired yaw/pitch, honoring vehicle-seat constraints:
 *   - Yaw: advanced by delta_yaw; if the occupied seat has a limited yaw arc (a "boarding"/turret seat),
 *     the new yaw is clamped into the arc relative to the seat marker's facing, then wrapped to [0, 2pi).
 *   - Pitch: optionally eased toward the seat camera's autolevel target, with the pitch limits (and the
 *     autolevel target) shifted by the angle between the camera forward and the unit forward when the
 *     seat is "locked"; the eased limits then bracket the new pitch (advanced by delta_pitch).
 *
 * Unit-object velocity/up-vector fields are read through float* windows into the typed members,
 * reproducing the decompiler's mixed-component speed math. */

#include <stdint.h>
#include "headers/player_control_globals.h"
#include "headers/unit_camera_info.h"
#include "headers/object_marker.h"
#include "headers/real_euler_angles2d.h"
#include "headers/real_vector3d.h"
#include "headers/global_tag_instances.h"
#include "headers/game_globals_tag.h"
#include "headers/game_globals_player_control.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/unit_seat.h"
#include "headers/blam_data_globals.h"
#include "headers/math_constants.h"

extern void player_control_get_unit_camera_info(int16_t local_player_index, unit_camera_info *camera_info);
extern int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern real_euler_angles2d *euler_angles2d_from_vector3d(real_euler_angles2d *angles, const real_vector3d *vector);
extern real_vector3d *vector3d_from_euler_angles2d(real_vector3d *vector, const real_euler_angles2d *angles);
extern float angle_between_vectors3d(const real_vector3d *a, const real_vector3d *b);
extern double __fabs(double x);
extern float  __fsqrts(float x);

/* yaw wrap helper: bring an angle difference into (-pi, pi] then, for the arc test, into [0, 2pi) */
static double wrap_pi(double a)
{
    if ( a >= PI )      a = (float)(a - TWO_PI);
    if ( a <= -3.1415927 )     a = (float)(a + TWO_PI);
    return a;
}

void player_control_modify_desired_angles(int16_t local_player_index, float delta_yaw, float delta_pitch)
{
    double pitch_minimum = -1.4922565;
    double pitch_maximum = 1.4922565;
    game_globals_player_control *player_control_tag = (game_globals_player_control *)global_game_globals->player_control.address;
    player_control *control = &player_control_globals->players[local_player_index];
    unit_camera_info camera_info;
    int seat_index;

    player_control_get_unit_camera_info(local_player_index, &camera_info);
    seat_index = camera_info.seat_index;

    control->desired_angles.n[0] = control->desired_angles.n[0] + delta_yaw;   /* delta_yaw is float (fadds direct, no frsp) */

    if ( seat_index != -1 )
    {
        /* unit object data pointer (object header datum: data ptr at int element 3*idx+2) */
        unit_datum *unit_object = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, camera_info.unit_index)->datum);
        unit_definition *unit_def = TAG_GET(unit_definition, unit_object->definition_index);
        unit_seat *seat_def = (unit_seat *)unit_def->unit.seats.address + seat_index;
        float yaw_min = seat_def->yaw_minimum;   /* arc start offset */
        float yaw_max = seat_def->yaw_maximum;   /* arc end offset   */

        if ( yaw_min != 0.0f || yaw_max != 0.0f )
        {
            object_marker seat_marker;
            real_euler_angles2d marker_angles;
            double arc_width, to_max, to_min;

            object_get_marker_by_name(camera_info.unit_index, seat_def->marker_name, &seat_marker, 1);
            /* the marker matrix's first row (float[3]) is its forward vector; reinterpret as a vector3d. */
            euler_angles2d_from_vector3d(&marker_angles, (const real_vector3d *)&seat_marker.matrix.n[0]);

            arc_width = wrap_pi(((yaw_max + marker_angles.n[0]) - (yaw_min + marker_angles.n[0])));
            to_max    = wrap_pi(((yaw_max + marker_angles.n[0]) - control->desired_angles.n[0]));
            to_min    = wrap_pi((control->desired_angles.n[0] - (yaw_min + marker_angles.n[0])));

            if ( arc_width < 0.0 )
                arc_width = (float)(arc_width + TWO_PI);

            /* outside the arc: snap to whichever edge is nearer */
            if ( (to_max < 0.0 || to_max >= arc_width) && (to_min < 0.0 || to_min >= arc_width) )
            {
                if ( __fabs(to_min) >= __fabs(to_max) )
                    control->desired_angles.n[0] = yaw_max + marker_angles.n[0];
                else
                    control->desired_angles.n[0] = yaw_min + marker_angles.n[0];
            }
        }
    }

    /* wrap yaw into [0, 2pi) */
    while ( control->desired_angles.n[0] < 0.0f )
        control->desired_angles.n[0] = control->desired_angles.n[0] + TWO_PI;
    while ( control->desired_angles.n[0] > TWO_PI )
        control->desired_angles.n[0] = control->desired_angles.n[0] - TWO_PI;

    if ( camera_info.unit_camera )
    {
        double pitch_autolevel = camera_info.unit_camera->pitch_autolevel;
        unit_datum *unit_object = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, camera_info.unit_index)->datum);

        if ( camera_info.unit_camera->pitch_maximum != 0.0f || camera_info.unit_camera->pitch_minimum != 0.0f )
        {
            pitch_minimum = camera_info.unit_camera->pitch_minimum;
            pitch_maximum = camera_info.unit_camera->pitch_maximum;

            /* unit mostly upright (up.z > 0.2): bias limits by look-vs-unit-up angle */
            if ( (uint16_t)camera_info.seat_index != 0xFFFF && unit_object->object.up.n[2] > 0.2f )
            {
                real_euler_angles2d look_angles;
                real_vector3d look_vector;
                double tilt;

                look_angles.n[0] = control->desired_angles.n[0];
                look_angles.n[1] = 0.0f;
                vector3d_from_euler_angles2d(&look_vector, &look_angles);
                tilt = angle_between_vectors3d(&look_vector, &unit_object->object.up);

                pitch_minimum  = (float)(pitch_minimum  - (1.5707964f - (float)tilt));
                pitch_maximum  = (float)(pitch_maximum  - (1.5707964f - (float)tilt));
                pitch_autolevel = (float)(pitch_autolevel - (1.5707964f - (float)tilt));
            }

            if ( pitch_minimum >= -1.4922565 )  pitch_minimum = pitch_minimum > 1.4922565 ? 1.4922565 : pitch_minimum;
            else                                pitch_minimum = -1.4922565;
            if ( pitch_maximum >= -1.4922565 )  pitch_maximum = pitch_maximum > 1.4922565 ? 1.4922565 : pitch_maximum;
            else                                pitch_maximum = -1.4922565;
        }

        if ( pitch_autolevel != 0.0 || control->use_autolevel )
        {
            /* ease pitch toward the autolevel target, rate scaled by unit angular/linear speed */
            double error_frac = (float)(__fabs((control->desired_angles.n[1] - (float)pitch_autolevel))
                                        * 0.6366197466850281);   /* 2/pi */
            double max_step;
            double toward;
            /* float windows: a = translational_velocity.xyz; b starts at velocity.z (b[1],b[2] spill into forward.xy) */
            float *unit_vec_a = unit_object->object.translational_velocity.n;
            float *unit_vec_b = &unit_object->object.translational_velocity.n[2];

            if ( pitch_autolevel == 0.0 )
                max_step = ((__fsqrts(((unit_vec_a[0] * unit_vec_a[0])
                                            + ((unit_vec_a[1] * unit_vec_a[1])
                                                    + (unit_vec_a[2] * unit_vec_a[2]))))
                                    * player_control_tag->look_autolevel_scale) * (float)error_frac);
            else
                max_step = ((__fsqrts(((unit_vec_b[0] * unit_vec_b[0])
                                            + ((unit_vec_a[0] * unit_vec_a[0])
                                                    + (unit_vec_a[1] * unit_vec_a[1]))))
                                    * (float)error_frac) * 0.079999998f);

            toward = ((float)pitch_autolevel - control->desired_angles.n[1]);
            if ( toward >= -max_step )  toward = toward > max_step ? max_step : toward;
            else                        toward = -max_step;
            control->desired_angles.n[1] = control->desired_angles.n[1] + (float)toward;
        }
    }

    /* ease the stored pitch limits toward this frame's targets (max 0.0123 rad/frame) */
    {
        double dmin = ((float)pitch_minimum - control->pitch_minimum);
        double dmax;
        if ( dmin >= -0.012271847 )  dmin = dmin > 0.012271847 ? 0.012271847 : dmin;
        else                         dmin = -0.012271847;
        control->pitch_minimum = control->pitch_minimum + (float)dmin;

        dmax = ((float)pitch_maximum - control->pitch_maximum);
        if ( dmax >= -0.012271847 )  dmax = dmax > 0.012271847 ? 0.012271847 : dmax;
        else                         dmax = -0.012271847;
        control->pitch_maximum = control->pitch_maximum + (float)dmax;
    }

    /* apply pitch delta and clamp into [pitch_minimum, pitch_maximum] */
    {
        double new_pitch = (control->desired_angles.n[1] + delta_pitch);   /* delta_pitch is float */
        if ( new_pitch >= control->pitch_minimum && new_pitch <= control->pitch_maximum )
            control->desired_angles.n[1] = (float)new_pitch;
        else if ( new_pitch < control->pitch_minimum )
            control->desired_angles.n[1] = control->pitch_minimum;
        else
            control->desired_angles.n[1] = control->pitch_maximum;
    }
}
