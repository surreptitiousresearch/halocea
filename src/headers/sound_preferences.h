#pragma once

/* Per-platform sound channel preferences. */
typedef struct sound_preferences
{
    short platform_code;            // 0x00
    short actual_channel_counts[4]; // 0x02
    short virtual_channel_counts[4];// 0x0A
} sound_preferences;

extern sound_preferences default_sound_preferences;
