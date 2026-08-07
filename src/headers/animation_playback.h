#pragma once
#include <stdint.h>
/* animation_playback — event-stream codec (initialize + apply) used by recorded_animation_play_internal.
 * DB types_members animation_playback. Fnptr declarations kept single-line (verifier parse requirement). */
#include "unit_control_data.h"
#include "animation_playback_controller.h"
typedef struct animation_playback
{
    void (*initialize_event_stream)(animation_playback_controller *controller, unit_control_data *control, const char **stream, uint8_t init_parameter); /* 0x00 */
    uint8_t (*apply_event_stream)(animation_playback_controller *controller, unit_control_data *control, int *stream_cursor, const char **stream_data); /* 0x04 */
} animation_playback;
