/* platform_sound_channel_properties — the per-channel parameter block handed to the platform sound
 * layer when a channel is started or updated (32 bytes). */
#pragma once

typedef struct platform_sound_channel_properties
{
    float minimum_distance;      // 0x00
    float maximum_distance;      // 0x04
    float pitch;                 // 0x08
    float gain;                  // 0x0C
    float inner_cone_angle;      // 0x10
    float outer_cone_angle;      // 0x14
    float outer_cone_gain;       // 0x18
    float reverb_damping_factor; // 0x1C
} platform_sound_channel_properties;
