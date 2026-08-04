#pragma once
/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* animation_playback_codec — apply-only event-stream codec used by recorded_animations_update. */

typedef struct animation_playback_controller animation_playback_controller;
typedef struct unit_control_data unit_control_data;

typedef struct animation_playback_codec
{
    int (*apply_event_stream)(animation_playback_controller *controller, unit_control_data *control,
                              int *stream_cursor, const char **stream_data);
} animation_playback_codec;
