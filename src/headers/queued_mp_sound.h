#pragma once
/* queued_mp_sound — one entry in the multiplayer queued-sound ring (16 bytes). */

typedef struct queued_mp_sound
{
    int             player_index;     /* 0x00 */
    int             index;            /* 0x04 */
    int             ticks_left;       /* 0x08 */
    unsigned __int8 should_replicate; /* 0x0C */
    unsigned __int8 _pad[3];          /* 0x0D */
} queued_mp_sound;
