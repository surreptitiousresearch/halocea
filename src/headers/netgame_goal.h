#pragma once
/* netgame_goal — one multiplayer objective/goal marker (32 bytes). Layout from the database. */

#include "real_point3d.h"

typedef struct netgame_goal
{
    real_point3d    position;            /* 0x00 */
    unsigned __int8 in_use;              /* 0x0C */
    char            _pad0D[3];           /* 0x0D */
    int             player_index;        /* 0x10 */
    __int16         team_index;          /* 0x14 */
    char            _pad16[2];           /* 0x16 */
    int             ignore_player_index; /* 0x18 */
    __int16         nav_index;           /* 0x1C */
    char            _pad1E[2];           /* 0x1E */
} netgame_goal;                          /* 0x20 */
