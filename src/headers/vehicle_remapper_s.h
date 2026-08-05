#pragma once
/* vehicle_remapper_s — runtime state for the multiplayer vehicle remapper, which substitutes scenario vehicles
 * according to the active game variant's vehicle set. Full nested layout from the database (964 bytes). */

#include <stdint.h>
#include "vehicle_team_spawn_info_s.h"

typedef struct vehicle_remapper_s
{
    vehicle_team_spawn_info_s vehicles[6][2]; /* 0x000 */
    uint8_t           processed;       /* 0x3C0 */
    uint8_t           is_teamplay;     /* 0x3C1 */
} vehicle_remapper_s;                          /* 964 bytes */

extern vehicle_remapper_s vehicle_remapper;
