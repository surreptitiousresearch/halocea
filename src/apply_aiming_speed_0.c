#include "headers/animation_playback_controller.h"
#include "headers/unit_control_data.h"
#include "headers/animation_event_header.h"

void apply_aiming_speed_0(animation_playback_controller *animation_state,
                           unit_control_data *control,
                           const animation_event_header *header,
                           const char **playback_stream)
{
    control->aiming_speed = (unsigned char)*(*playback_stream)++;
}
