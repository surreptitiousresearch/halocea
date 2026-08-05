#pragma once
/* hud_timer_data_definition — HUD countdown/count-up timer state (16 bytes). DB-verified layout. */

#include <stdint.h>
#include "point2d.h"

typedef struct hud_timer_data_definition
{
    int           reference_time;   /* 0x0 */
    int16_t       ticks;            /* 0x4 */
    int16_t       flash_cutoff;     /* 0x6 */
    point2d       position;         /* 0x8 */
    int16_t       corner;           /* 0xC */
    unsigned char paused;           /* 0xE */
    unsigned char enabled;          /* 0xF */
} hud_timer_data_definition;        /* 0x10 (16 bytes) */
