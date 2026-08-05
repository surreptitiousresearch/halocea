#pragma once
/* player_update — one queued networked player update (a node in a player_update_history list): the update id,
 * the tick it applies to, the player_action input, and a full biped (and, when driving, vehicle) state snapshot
 * captured when the update was added, used for client-side prediction and reconciliation. Layout from the
 * database (0x418 bytes). */

#include <stdint.h>
#include "player_action.h"
#include "biped_state.h"
#include "vehicle_state.h"

typedef struct player_update
{
    int                   id;                        /* 0x000 */
    int                   ticks_to_apply_update_to;  /* 0x004 */
    player_action         action;                    /* 0x008 */
    uint8_t       was_vehicle_driver;        /* 0x028 */
    char                  _pad029[3];                /* 0x029 */
    biped_state           biped_initial_state;       /* 0x02C */
    vehicle_state         vehicle_initial_state;     /* 0x100 */
    struct player_update *next;                      /* 0x414 */
} player_update;                                      /* 0x418 bytes */
