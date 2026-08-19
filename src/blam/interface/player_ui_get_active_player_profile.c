/* player_ui_get_active_player_profile @0x83698A40 — copy a local player's active profile out of the player UI
 * globals slot. */

#include <stdint.h>
#include <string.h>
#include "headers/player_ui_globals.h"


void player_ui_get_active_player_profile(int16_t local_player_index, player_profile *profile)
{
    memcpy(profile, &player_ui_globals.player_data[local_player_index], sizeof(player_profile));
}
