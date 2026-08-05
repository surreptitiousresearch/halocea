#pragma once
/* scenario_player — one player starting location in the scenario tag (52 bytes). */

#include <stdint.h>
#include "real_point3d.h"

typedef struct scenario_player
{
    real_point3d position;     /* 0x00 */
    float        facing;       /* 0x0C */
    int16_t      team_index;   /* 0x10 */
    int16_t      bsp_index;    /* 0x12 */
    int16_t      game_type[4]; /* 0x14 */
    int          unused[6];    /* 0x1C */
} scenario_player;             /* 52 bytes */
