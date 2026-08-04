#pragma once
/* scenario_player — one player starting location in the scenario tag (52 bytes). */

#include "real_point3d.h"

typedef struct scenario_player
{
    real_point3d position;     /* 0x00 */
    float        facing;       /* 0x0C */
    __int16      team_index;   /* 0x10 */
    __int16      bsp_index;    /* 0x12 */
    __int16      game_type[4]; /* 0x14 */
    int          unused[6];    /* 0x1C */
} scenario_player;             /* 52 bytes */
