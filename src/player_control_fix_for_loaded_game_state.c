/* player_control_fix_for_loaded_game_state @0x836A8890 — after loading a saved single-player game state,
 * re-bind the (possibly stale) single-player local-player-controller slot to whichever player datum is
 * actually still occupying a local-player slot: only runs when the controller's own slot looks unbound and
 * exactly one player has ever spawned. Scans local_player_indices for the first bound player, clears its
 * old unit, rebinds it to the correct controller, restores its unit, and fixes up HUD weapon/unit caches.
 *
 * SHIPPED QUIRK: the scan bound is 4, but `players_global_data.local_player_indices` (DB-confirmed) is
 * only a 2-element array — reaching scan index 2 or 3 reads into the adjacent
 * `local_player_failed_teleport_unit_indices` field instead. Reproduced verbatim. */

#include <stdint.h>
#include "headers/players_global_data.h"
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"


extern int16_t player_ui_get_single_player_local_player_controller(int16_t local_player_index);
extern void player_control_new_unit(int16_t local_player_index, int unit_index);
extern void local_player_set_player_index(int16_t local_player_index, int player_index);
extern void hud_fix_weapon_data(int16_t old_local_player_index, int16_t new_local_player_index);
extern void hud_fix_unit_data(int16_t old_local_player_index, int16_t new_local_player_index);

void player_control_fix_for_loaded_game_state(void)
{
    __int16 requested_controller = player_ui_get_single_player_local_player_controller(0);
    __int16 controller = requested_controller == -1 ? 0 : requested_controller;

    if ( (controller == -1 || players_globals->local_player_indices[controller] == -1)
      && player_spawn_count == 1 )
    {
        __int16 scan_index = 0;
        int player_index;

        while ( 1 )
        {
            player_index = -1;
            if ( scan_index != -1 )
                player_index = players_globals->local_player_indices[scan_index];

            if ( player_index != -1 )
                break;

            scan_index = (__int16)(scan_index + 1);
            if ( scan_index >= 4 )
                return;
        }

        player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
        player->local_player_index = -1;
        players_globals->local_player_indices[scan_index] = -1;

        player_control_new_unit(scan_index, -1);
        local_player_set_player_index(controller, player_index);
        player_control_new_unit(scan_index, player->unit_index);
        hud_fix_weapon_data(scan_index, controller);
        hud_fix_unit_data(scan_index, controller);
    }
}
