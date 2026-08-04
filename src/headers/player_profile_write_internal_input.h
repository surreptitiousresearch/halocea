#pragma once
/* player_profile_write_internal_input — input block for the async player-profile write thread.
 * Layout per database (types_members player_profile_write_internal_input). */

#include "player_profile.h"

typedef struct player_profile_write_internal_input
{
    int            index;   /* 0x000 */
    player_profile profile; /* 0x004 */
} player_profile_write_internal_input; /* 1984 bytes */
