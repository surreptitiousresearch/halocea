#pragma once
/* sound_datum — one live impulse sound in the sound_data array (sound.c). 176 bytes,
 * DB types_members-confirmed layout. */

#include "sound_source.h"

typedef struct sound_datum
{
    __int16          identifier;             /* 0x00 */
    __int16          type;                   /* 0x02 */
    unsigned __int16 flags;                  /* 0x04 */
    __int16          listener_index;         /* 0x06 */
    int              definition_index;       /* 0x08 */
    int              source_identifier;      /* 0x0C */
    unsigned __int8 (*track_proc)(int sound_index, const void *track_data, sound_source *source); /* 0x10 */
    sound_source     source;                 /* 0x14 */
    unsigned __int8  track_data[48];         /* 0x54 */
    int              start_time;             /* 0x84 */
    float            pitch;                  /* 0x88 */
    __int16          playing_channel_index;  /* 0x8C */
    __int16          pitch_range_index;      /* 0x8E */
    __int16          permutation_index;      /* 0x90 */
    __int16          fade_mode;              /* 0x92 */
    __int16          loop_track_index;       /* 0x94 */
    unsigned char _pad0[2]; /* db-verified padding */
    int              next_definition_index;  /* 0x98 */
    float            fade_interpolation_start; /* 0x9C */
    float            fade_interpolation_end;   /* 0xA0 */
    int              fade_start_time;        /* 0xA4 */
    int              fade_stop_time;         /* 0xA8 */
    unsigned __int8  is_local_player;        /* 0xAC */
} sound_datum;                               /* 176 bytes */
