#pragma once
/* looping_sound_detail_datum — per-detail runtime state inside looping_sound_datum (sound.c).
 * 4 bytes, DB types_members-confirmed. */

typedef struct looping_sound_detail_datum
{
    int next_play_time; /* 0x00 */
} looping_sound_detail_datum;
