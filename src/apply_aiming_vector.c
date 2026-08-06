/* apply_aiming_vector @ 0x837F6A90 — recorded-animation v1 event apply:
   copies the 3-float aiming vector out of the event payload and advances the stream. */
#include "headers/unit_control_data.h"
#include "headers/animation_event_v1.h"
#include "headers/real_vector3d.h"

void apply_aiming_vector(unit_control_data *control, const animation_event_v1 *anim_event_v1, const char **playback_stream)
{
    /* the payload starts one event header past the event: lwz 4/8/0xC(r4) -> stw 0x28/0x2C/0x30(r3)
     * @0x837F6A90, and the stream advances by header+payload (addi r8, r11, 0x10). */
    control->aiming_vector = *(const real_vector3d *)&anim_event_v1[1];
    *playback_stream += sizeof(animation_event_v1) + sizeof(real_vector3d);
}
