/* apply_multi_vector @0x837F6B78 — apply an animation event's trailing real_vector3d payload (immediately
 * following the 4-byte animation_event_v1 header) to whichever of the unit's facing/aiming/looking vectors
 * the event type does NOT equal, then advance the playback stream past the 16-byte event. */

#include "headers/unit_control_data.h"
#include "headers/animation_event_v1.h"
#include "headers/real_vector3d.h"
#include "headers/playback_v1_event_type.h"

void apply_multi_vector(unit_control_data *control, const animation_event_v1 *anim_event_v1,
    const char **playback_stream)
{
    const real_vector3d *vector = (const real_vector3d *)(anim_event_v1 + 1);

    if ( anim_event_v1->type != _playback_v1_aiming_looking_vector_set )
        control->facing_vector = *vector;
    if ( anim_event_v1->type != _playback_v1_facing_looking_vector_set )
        control->aiming_vector = *vector;
    if ( anim_event_v1->type != _playback_v1_facing_aiming_vector_set )
        control->looking_vector = *vector;

    *playback_stream += 16;
}
