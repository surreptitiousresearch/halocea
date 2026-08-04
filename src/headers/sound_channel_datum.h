#pragma once

typedef struct sound_permutation sound_permutation;

typedef struct sound_channel_datum
{
    int sound_index;                    // 0x00
    short type_flags;                   // 0x04
    unsigned char _pad0[2]; /* db-verified padding */
    float estimated_tick_time;          // 0x08
    float pitch;                        // 0x0C
    sound_permutation *playing_permutation; // 0x10
    sound_permutation *queued_permutation;  // 0x14
} sound_channel_datum;
