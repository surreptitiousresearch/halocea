/* player_update_client_is_position_tracked_remote_player @0x8376DB78 — true when this remote player's name
 * matches the debug "track position updates" filter, used to gate verbose position-update logging. */

#include <stdint.h>
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"

extern int ustrcmp(const wchar_t *string1, const wchar_t *string2);

uint8_t player_update_client_is_position_tracked_remote_player(player_datum *remote_player)
{
    return ustrcmp(remote_player->name, g_tracked_remote_player_position_updates) == 0;
}
