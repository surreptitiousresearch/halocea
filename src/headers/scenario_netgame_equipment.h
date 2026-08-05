#pragma once
/* scenario_netgame_equipment — one entry in a scenario's netgame_equipment block (144-byte element). Describes a
 * multiplayer item/weapon spawn point: which game types it appears in, where it sits, its respawn period, and the
 * item_collection it draws from. Layout from the database. */

#include <stdint.h>
#include "real_point3d.h"
#include "tag_reference.h"

typedef struct scenario_netgame_equipment
{
    int           flags;                       /* 0x00 — bit0: align-to-collection / placement flag */
    int16_t       game_type[4];                /* 0x04 — game-type filter (see match_game_type) */
    int16_t       team_index;                  /* 0x0C */
    int16_t       spawn_time;                  /* 0x0E — respawn period in seconds; 0 = use collection default */
    int           run_time_spawned_item_index; /* 0x10 — currently spawned object index, -1 if none */
    int           unused1[11];                 /* 0x14 */
    real_point3d  position;                    /* 0x40 */
    float         facing;                      /* 0x4C */
    tag_reference item_collection;             /* 0x50 — .index drives random_item() */
    int           unused2[12];                 /* 0x60 */
} scenario_netgame_equipment;                  /* 144 bytes */
