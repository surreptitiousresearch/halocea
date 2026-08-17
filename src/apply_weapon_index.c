/* apply_weapon_index @0x837F6A20 */
#include "headers/unit_control_data.h"
#include "headers/animation_event_v1.h"

void apply_weapon_index(unit_control_data *control,
                         const animation_event_v1 *anim_event_v1,
                         const char **playback_stream)
{
    control->weapon_index = anim_event_v1[1].type;
    *playback_stream += 6;
}
