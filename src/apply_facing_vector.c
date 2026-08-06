/* apply_facing_vector @ 0x837F6A68 — recorded-animation v1 event apply:
   copies the 3-float facing vector out of the event payload and advances the stream. */
#include "headers/unit_control_data.h"
#include "headers/animation_event_v1.h"
#include "headers/real_vector3d.h"

void apply_facing_vector(unit_control_data *control, const animation_event_v1 *anim_event_v1, const char **playback_stream)
{
    /* the payload starts one event header past the event: lwz 4/8/0xC(r4) -> stw 0x1C/0x20/0x24(r3)
     * @0x837F6A68, and the stream advances by header+payload (addi r8, r11, 0x10). */
    control->facing_vector = *(const real_vector3d *)&anim_event_v1[1];
    *playback_stream += sizeof(animation_event_v1) + sizeof(real_vector3d);
}
