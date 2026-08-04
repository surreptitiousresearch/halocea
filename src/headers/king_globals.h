#pragma once
/* king_globals_type — King of the Hill game-engine state (428 bytes, DB types_members-confirmed layout). */

#include "real_point2d.h"
#include "real_point3d.h"

typedef struct king_globals_type
{
    int              score[16];               /* 0x000 */
    int              score_tick[16];           /* 0x040 */
    unsigned __int8  on_the_hill[16];           /* 0x080 */
    int              hill_point_count;          /* 0x090 */
    real_point3d     hill_points[12];            /* 0x094 */
    real_point2d     convex_hull[12];             /* 0x124 */
    real_point3d     hill_center;                 /* 0x184 */
    int              hill_state;                   /* 0x190 */
    int              hill_controlled_count;         /* 0x194 */
    int              hill_previous_controller;       /* 0x198 */
    float            hill_top;                        /* 0x19C */
    float            hill_bottom;                      /* 0x1A0 */
    int              hill_id;                            /* 0x1A4 */
    int              hill_timer;                          /* 0x1A8 */
} king_globals_type; /* 428 bytes */

extern king_globals_type king_globals;
