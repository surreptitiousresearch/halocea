#pragma once
/* sound_datum — one live impulse sound in the sound_data array (sound.c). 176 bytes,
 * DB types_members-confirmed layout. */

#include <stdint.h>
#include "sound_source.h"

typedef struct sound_datum
{
    int16_t          identifier;             /* 0x00 */
    int16_t          type;                   /* 0x02 */
    uint16_t flags;                  /* 0x04 */
    int16_t          listener_index;         /* 0x06 */
    int              definition_index;       /* 0x08 */
    int              source_identifier;      /* 0x0C */
    uint8_t (*track_proc)(int sound_index, const void *track_data, sound_source *source); /* 0x10 */
    sound_source     source;                 /* 0x14 */
    uint8_t  track_data[48];         /* 0x54 */
    int              start_time;             /* 0x84 */
    float            pitch;                  /* 0x88 */
    int16_t          playing_channel_index;  /* 0x8C */
    int16_t          pitch_range_index;      /* 0x8E */
    int16_t          permutation_index;      /* 0x90 */
    int16_t          fade_mode;              /* 0x92 */
    int16_t          loop_track_index;       /* 0x94 */
    unsigned char _pad0[2]; /* db-verified padding */
    int              next_definition_index;  /* 0x98 */
    float            fade_interpolation_start; /* 0x9C */
    float            fade_interpolation_end;   /* 0xA0 */
    int              fade_start_time;        /* 0xA4 */
    int              fade_stop_time;         /* 0xA8 */
    uint8_t  is_local_player;        /* 0xAC */
} sound_datum;                               /* 176 bytes */
