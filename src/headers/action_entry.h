#pragma once
/* action_entry — a buffered player action to replay (44 bytes). Both tick fields start at
 * ticks_to_apply_update_to; remaining_ticks_to_apply_action_to is decremented as the action is
 * consumed. Layout from the database. */

#include "player_action.h"

typedef struct action_entry
{
    int           client_update_id;                   /* 0x00 */
    int           remaining_ticks_to_apply_action_to; /* 0x04 */
    int           total_ticks_to_apply_action_to;     /* 0x08 */
    player_action action;                             /* 0x0C */
} action_entry;                                       /* 44 bytes */
