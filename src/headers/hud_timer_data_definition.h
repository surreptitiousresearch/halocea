#pragma once
/* hud_timer_data_definition — HUD countdown/count-up timer state (16 bytes). DB-verified layout. */

#include "point2d.h"

typedef struct hud_timer_data_definition
{
    int           reference_time;   /* 0x0 */
    __int16       ticks;            /* 0x4 */
    __int16       flash_cutoff;     /* 0x6 */
    point2d       position;         /* 0x8 */
    __int16       corner;           /* 0xC */
    unsigned char paused;           /* 0xE */
    unsigned char enabled;          /* 0xF */
} hud_timer_data_definition;        /* 0x10 (16 bytes) */
