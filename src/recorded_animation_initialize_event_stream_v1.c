#include <stdint.h>
#include "headers/animation_playback_controller.h"
#include "headers/unit_control_data.h"

extern void recorded_animation_initialize_unit_control(unit_control_data *control, const char **playback_stream, uint8_t unit_version);

void recorded_animation_initialize_event_stream_v1(animation_playback_controller *animation_state, unit_control_data *control, const char **playback_stream, uint8_t unit_control_version)
{
    recorded_animation_initialize_unit_control(control, playback_stream, unit_control_version);
}
