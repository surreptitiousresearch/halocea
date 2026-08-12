/* apply_throttle_0 @ 0x837F6F08 — recorded-animation (header-form) event apply:
   copies an 8-byte (two-float) throttle from the payload, zeroes z, advances stream by 8. */
#include "headers/animation_playback_controller.h"
#include "headers/unit_control_data.h"
#include "headers/animation_event_header.h"

void apply_throttle_0(animation_playback_controller *animation_state, unit_control_data *control, const animation_event_header *header, const char **playback_stream)
{
    const char *next = *playback_stream + 8;
    long long throttle_xy = *(const long long *)*playback_stream;
    /* DEVIATION: the payload reaches control->throttle as TWO 4-byte float stores, not one
       8-byte store. The binary spills the doubleword to the frame and reloads it as a pair of
       floats: `std r8, back_chain(r1)` @0x837F6F24, `lfs f13, back_chain+4(r1)` @0x837F6F28,
       `lfs f0, back_chain(r1)` @0x837F6F2C, then `stfs f0, 0xC(r4)` @0x837F6F30 and
       `stfs f13, 0x10(r4)` @0x837F6F34. An `*(long long *)control->throttle.n = ...` here also
       INVENTS an alignment requirement the original never had: throttle sits at +0x0C of a
       4-aligned unit_control_data, so the doubleword store it asks for can be misaligned where
       the two stfs never could. */
    const float *throttle_pair = (const float *)&throttle_xy;
    control->throttle.n[2] = 0.0f;
    *playback_stream = next;
    control->throttle.n[0] = throttle_pair[0];
    control->throttle.n[1] = throttle_pair[1];
}
