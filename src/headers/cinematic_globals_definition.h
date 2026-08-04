#pragma once
/* cinematic_globals_definition @ cinematic.c — cinematic playback state. */

#include "cinematic_title_datum.h"

typedef struct cinematic_globals_definition
{
    float         letter_box_amount;                       /* 0x00 */
    int           letter_box_last_game_time;               /* 0x04 */
    unsigned char letter_box;                              /* 0x08 */
    unsigned char cinematic_in_progress;                   /* 0x09 */
    unsigned char cinematic_skip_in_progress;              /* 0x0A */
    unsigned char cinematic_suppress_bsp_object_creation;  /* 0x0B */
    cinematic_title_datum active_titles[4];                /* 0x0C */
} cinematic_globals_definition; /* 28 bytes */
