#pragma once
/* looping_sound_datum — one live looping sound in the looping_sound_data array (sound.c).
 * 228 bytes, DB types_members-confirmed layout. */

#include <stdint.h>
#include "sound_source.h"
#include "looping_sound_detail_datum.h"
#include "looping_sound_track_datum.h"

typedef struct looping_sound_datum
{
    int16_t                    identifier;                    /* 0x00 */
    uint16_t           pad;                           /* 0x02 */
    int                        definition_index;              /* 0x04 */
    int                        loop_identifier;               /* 0x08 */
    sound_source               source;                        /* 0x0C */
    uint8_t            flip_flop;                     /* 0x4C */
    uint8_t            alternate;                     /* 0x4D */
    uint8_t            ordered_permutations_finished; /* 0x4E */
    unsigned char _pad0[1]; /* db-verified padding */
    int16_t                    component_sound_count;         /* 0x50 */
    int16_t                    state;                         /* 0x52 */
    looping_sound_detail_datum details[32];                   /* 0x54 */
    looping_sound_track_datum  tracks[4];                     /* 0xD4 */
} looping_sound_datum;                                        /* 228 bytes */
