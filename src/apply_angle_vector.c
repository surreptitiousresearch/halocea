/* apply_angle_vector @0x837F6AE0 — applies a 2D animation-event angle (converted to a unit vector) to a
 * unit's facing/aiming/looking vectors, skipping whichever one the event's own type omits (the paired
 * _playback_v1_*_angles_set codes name the two vectors they set; the excluded vector is the one skipped:
 * 21=aiming+looking skips facing, 20=facing+looking skips aiming, 19=facing+aiming skips looking). The
 * other two vectors are overwritten with the same converted vector. Advances the stream past the 12-byte
 * angle event payload. */

#include "headers/unit_control_data.h"
#include "headers/animation_event_v1.h"
#include "headers/real_vector3d.h"
#include "headers/real_euler_angles2d.h"
#include "headers/playback_v1_event_type.h"

extern real_vector3d *vector3d_from_euler_angles2d(real_vector3d *vector, const real_euler_angles2d *angles);

void apply_angle_vector(unit_control_data *control, const animation_event_v1 *anim_event_v1, const char **playback_stream)
{
    real_vector3d angle_vector;
    vector3d_from_euler_angles2d(&angle_vector, (const real_euler_angles2d *)&anim_event_v1[1]);

    if ( anim_event_v1->type != _playback_v1_aiming_looking_angles_set )
        control->facing_vector = angle_vector;

    if ( anim_event_v1->type != _playback_v1_facing_looking_angles_set )
        control->aiming_vector = angle_vector;

    if ( anim_event_v1->type != _playback_v1_facing_aiming_angles_set )
        control->looking_vector = angle_vector;

    *playback_stream += 12;
}
