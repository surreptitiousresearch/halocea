/* apply_control_flags @0x837F6A08 */
#include "headers/unit_control_data.h"
#include "headers/animation_event_v1.h"

void apply_control_flags(unit_control_data *control,
                          const animation_event_v1 *anim_event_v1,
                          const char **playback_stream)
{
    control->control_flags = anim_event_v1[1].type;
    *playback_stream += 6;
}
