#pragma once
/* vehicle_team_spawn_info_s — per-team vehicle spawn bookkeeping for the multiplayer
 * vehicle remapper (DB types_members-confirmed, 80 bytes). */

#include "vehicle_info_s.h"

typedef struct vehicle_team_spawn_info_s
{
    unsigned int   map_count;        /* 0x00 */
    unsigned int   num_spawned;      /* 0x04 */
    unsigned int   max_to_spawn;     /* 0x08 */
    unsigned int   ref_index;        /* 0x0C */
    vehicle_info_s team_vehicles[8]; /* 0x10 */
} vehicle_team_spawn_info_s;         /* 80 bytes */
