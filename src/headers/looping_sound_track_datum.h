#pragma once
/* looping_sound_track_datum — per-track runtime state inside looping_sound_datum (sound.c).
 * 4 bytes, DB types_members-confirmed. */

typedef struct looping_sound_track_datum
{
    int primary_sound_index; /* 0x00 */
} looping_sound_track_datum;
