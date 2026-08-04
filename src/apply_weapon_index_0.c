#include "headers/animation_playback_controller.h"
#include "headers/unit_control_data.h"
#include "headers/animation_event_header.h"

void apply_weapon_index_0(animation_playback_controller *animation_state,
                           unit_control_data *control,
                           const animation_event_header *header,
                           const char **playback_stream)
{
    control->weapon_index = *(unsigned short *)*playback_stream;
    *playback_stream += 2;
}
