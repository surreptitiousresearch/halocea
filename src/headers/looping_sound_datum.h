#pragma once
/* looping_sound_datum — one live looping sound in the looping_sound_data array (sound.c).
 * 228 bytes, DB types_members-confirmed layout. */

#include "sound_source.h"
#include "looping_sound_detail_datum.h"
#include "looping_sound_track_datum.h"

typedef struct looping_sound_datum
{
    __int16                    identifier;                    /* 0x00 */
    unsigned __int16           pad;                           /* 0x02 */
    int                        definition_index;              /* 0x04 */
    int                        loop_identifier;               /* 0x08 */
    sound_source               source;                        /* 0x0C */
    unsigned __int8            flip_flop;                     /* 0x4C */
    unsigned __int8            alternate;                     /* 0x4D */
    unsigned __int8            ordered_permutations_finished; /* 0x4E */
    unsigned char _pad0[1]; /* db-verified padding */
    __int16                    component_sound_count;         /* 0x50 */
    __int16                    state;                         /* 0x52 */
    looping_sound_detail_datum details[32];                   /* 0x54 */
    looping_sound_track_datum  tracks[4];                     /* 0xD4 */
} looping_sound_datum;                                        /* 228 bytes */
