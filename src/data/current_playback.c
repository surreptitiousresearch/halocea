/* current_playback @ 0x84177D14 -- second-order .data; animation_playback reconstructed from binary bytes. */
#include <stdint.h>
#include "../headers/animation_playback.h"

#include "headers/animation_playback_controller.h"
#include "headers/unit_control_data.h"
#include "headers/animation_playback_controller.h"
#include "headers/unit_control_data.h"
#include "headers/animation_playback_controller.h"
#include "headers/unit_control_data.h"
extern void recorded_animation_initialize_event_stream(animation_playback_controller *animation_state, unit_control_data *control, const char **playback_stream, uint8_t unit_control_version);
extern uint8_t recorded_animation_apply_event_stream(animation_playback_controller *animation_state, unit_control_data *control, int *ticks, const char **playback_stream);

animation_playback current_playback = {
    .initialize_event_stream = recorded_animation_initialize_event_stream,
    .apply_event_stream      = recorded_animation_apply_event_stream,
};
