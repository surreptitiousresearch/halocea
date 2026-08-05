#pragma once
/* scenario_netgame_flag — one placed netgame flag (teleporter/CTF/etc. marker) in scenario.netgame_flags
 * (148 bytes, DB layout via types_members). */

#include <stdint.h>
#include "real_point3d.h"

typedef struct scenario_netgame_flag
{
    real_point3d position;    /* 0x00 */
    float        facing;      /* 0x0C */
    int16_t      type;        /* 0x10 */
    int16_t      team_index;  /* 0x12 */
    int          unused[32];  /* 0x14 */
} scenario_netgame_flag;       /* 148 bytes */
