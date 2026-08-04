#pragma once
/* looping_sound_track — one track block within a 'lsnd' tag (sound.c). 160 bytes,
 * DB types_members-confirmed layout. */

#include "tag_reference.h"

typedef struct looping_sound_track
{
    unsigned int  flags;                /* 0x00 */
    float         gain;                 /* 0x04 */
    float         fade_in_duration;     /* 0x08 */
    float         fade_out_duration;    /* 0x0C */
    int           unused[8];            /* 0x10 */
    tag_reference start_sound;          /* 0x30 */
    tag_reference loop_sound;           /* 0x40 */
    tag_reference stop_sound;           /* 0x50 */
    int           unused2[8];           /* 0x60 */
    tag_reference alternate_loop_sound; /* 0x80 */
    tag_reference alternate_stop_sound; /* 0x90 */
} looping_sound_track;                  /* 160 bytes */
