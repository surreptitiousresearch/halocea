/* apply_throttle_0 @ 0x837F6F08 — recorded-animation (header-form) event apply:
   copies an 8-byte (two-float) throttle from the payload, zeroes z, advances stream by 8. */
#include "headers/animation_playback_controller.h"
#include "headers/unit_control_data.h"
#include "headers/animation_event_header.h"

void apply_throttle_0(animation_playback_controller *animation_state, unit_control_data *control, const animation_event_header *header, const char **playback_stream)
{
    const char *next = *playback_stream + 8;
    long long throttle_xy = *(const long long *)*playback_stream;
    control->throttle.n[2] = 0.0f;
    *playback_stream = next;
    *(long long *)control->throttle.n = throttle_xy;
}
