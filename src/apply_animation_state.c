#include "headers/unit_control_data.h"
#include "headers/animation_event_v1.h"

void apply_animation_state(unit_control_data *control,
                             const animation_event_v1 *anim_event_v1,
                             const char **playback_stream)
{
    control->animation_state = (unsigned char)(anim_event_v1[1].type >> 8);
    *playback_stream += 6;
}
