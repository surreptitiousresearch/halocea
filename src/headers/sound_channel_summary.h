/* sound_channel_summary — scratch census of currently-playing channels that match a candidate
 * sound, used to enforce per-definition and per-source instance limits (72 bytes). */
#pragma once
#include <stdint.h>

typedef struct sound_channel_summary
{
    int16_t like_definition_count;            // 0x00
    int16_t like_definition_channels[16];     // 0x02
    int16_t maximum_instance_count;           // 0x22
    int16_t like_source_count;                // 0x24
    int16_t like_source_channels[16];         // 0x26
    int16_t maximum_source_instance_count;    // 0x46
} sound_channel_summary;
