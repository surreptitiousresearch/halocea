/* hud_tick_shield @ 0x836D81D8 — decrement the HUD's cached shield-vitality reading for the local
 * player owning the given player datum, so the on-screen shield bar animates down by `amount` this
 * tick. No-op if the player datum is stale or has no assigned local-player HUD slot (local_player_index == -1). */

#include "headers/data_array.h"
#include "headers/unit_hud_globals.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"

extern void *datum_try_and_get(const data_array *data, int index);

void hud_tick_shield(int player_index, float amount)
{
    player_datum *player = datum_try_and_get(player_data, player_index);
    if ( player )
    {
        int local_player_index = player->local_player_index;
        if ( local_player_index != -1 )
            unit_hud_globals->hud_states[local_player_index].last_shield_vitality -= amount;
    }
}
