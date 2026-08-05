#pragma once
/* scenario_cutscene_title — one element of scenario.cutscene_chapter_titles (96 bytes). DB layout. */

#include <stdint.h>
#include "rectangle2d.h"

typedef struct scenario_cutscene_title
{
    int           flags;             /* 0x00 */
    char          name[32];          /* 0x04 */
    int           pad0;              /* 0x24 */
    rectangle2d   bounds;            /* 0x28 */
    int16_t       text_index;        /* 0x30 */
    int16_t       style;             /* 0x32 */
    int16_t       justification;     /* 0x34 */
    int16_t       pad1;              /* 0x36 */
    unsigned int  text_flags;        /* 0x38 */
    unsigned int  foreground_color;  /* 0x3C — packed ARGB */
    unsigned int  shadow_color;      /* 0x40 — packed ARGB */
    float         fade_in_time;      /* 0x44 */
    float         up_time;           /* 0x48 */
    float         fade_out_time;     /* 0x4C */
    int           unused[4];         /* 0x50 */
} scenario_cutscene_title; /* 96 bytes */
