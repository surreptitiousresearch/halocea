#pragma once
/* game_looping_sound_datum — one entry in the game_looping_sound_data array (game_sound.c).
 * 52 bytes, DB types_members-confirmed layout. */

#include "sound_attachment_data.h"

typedef struct game_looping_sound_datum
{
    __int16               identifier;               /* 0x00 */
    __int16               state;                    /* 0x02 */
    int                   flags;                    /* 0x04 */
    float                 scale;                    /* 0x08 */
    int                   definition_index;         /* 0x0C */
    int                   object_index;             /* 0x10 */
    int                   last_audible_frame_index; /* 0x14 */
    sound_attachment_data attachment;               /* 0x18 */
} game_looping_sound_datum;                         /* 52 bytes */
