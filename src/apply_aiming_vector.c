/* apply_aiming_vector @ 0x837F6A90 — recorded-animation v1 event apply:
   copies the 3-float aiming vector out of the event payload and advances the stream. */
#include "headers/unit_control_data.h"
#include "headers/animation_event_v1.h"

void apply_aiming_vector(unit_control_data *control, const animation_event_v1 *anim_event_v1, const char **playback_stream)
{
    control->aiming_vector = *(real_vector3d *)&anim_event_v1[1].type;
    *playback_stream += 16;
}
