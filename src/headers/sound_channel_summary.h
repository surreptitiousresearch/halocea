/* sound_channel_summary — scratch census of currently-playing channels that match a candidate
 * sound, used to enforce per-definition and per-source instance limits (72 bytes). */
#pragma once

typedef struct sound_channel_summary
{
    __int16 like_definition_count;            // 0x00
    __int16 like_definition_channels[16];     // 0x02
    __int16 maximum_instance_count;           // 0x22
    __int16 like_source_count;                // 0x24
    __int16 like_source_channels[16];         // 0x26
    __int16 maximum_source_instance_count;    // 0x46
} sound_channel_summary;
