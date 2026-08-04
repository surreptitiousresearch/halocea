#pragma once
/* animation_playback — event-stream codec (initialize + apply) used by recorded_animation_play_internal.
 * DB types_members animation_playback. Fnptr declarations kept single-line (verifier parse requirement). */

typedef struct animation_playback
{
    void (__fastcall *initialize_event_stream)(struct animation_playback_controller *controller, struct unit_control_data *control, const char **stream, unsigned __int8 init_parameter); /* 0x00 */
    unsigned __int8 (__fastcall *apply_event_stream)(struct animation_playback_controller *controller, struct unit_control_data *control, int *stream_cursor, const char **stream_data); /* 0x04 */
} animation_playback;
