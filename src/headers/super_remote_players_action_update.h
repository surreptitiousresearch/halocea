#pragma once
/* super_remote_players_action_update — accumulated super-remote player action updates queued for
 * network send (1800 bytes, DB layout). */

#include "super_remote_players_action_update_network_data.h"

typedef struct super_remote_players_action_update
{
    unsigned __int8                                 is_stateless;      /* 0x0 */
    unsigned __int8                                 pad_0x1[3];        /* 0x1 */
    int                                             number_of_updates; /* 0x4 */
    super_remote_players_action_update_network_data network_data;      /* 0x8 */
} super_remote_players_action_update;                                  /* 1800 bytes */
