/* apply_throttle @ 0x837F6A38 — recorded-animation v1 event apply:
   reads a 2-float throttle (x,y) from the payload, zeroes z, advances stream by 12. */
#include "headers/unit_control_data.h"
#include "headers/animation_event_v1.h"

void apply_throttle(unit_control_data *control, const animation_event_v1 *anim_event_v1, const char **playback_stream)
{
    const char *next = *playback_stream + 12;
    control->throttle.n[0] = *(float *)&anim_event_v1[1];
    control->throttle.n[1] = *(float *)&anim_event_v1[2];
    *playback_stream = next;
    control->throttle.n[2] = 0.0f;
}
