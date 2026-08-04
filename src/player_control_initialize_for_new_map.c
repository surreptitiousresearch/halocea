/* player_control_initialize_for_new_map @0x836DF520 — reset per-player control input state for a new map: clear
 * the global action/inhibit/control flags, then for each local player zero its control record and set the
 * unit/target/weapon/grenade/zoom sentinels (-1), pitch limits, and (if unset) the look pitch/yaw rates from
 * the game-globals player-control tuning.
 *
 * Deviation: the decompiler's "NAN" stores to unit_index/target_object_index are integer -1 stores (verified
 * via disasm: stw of the -1 register), not float NaNs. */

#include <stdint.h>
#include <string.h>
#include "headers/player_control_globals.h"
#include "headers/game_globals_tag.h"
#include "headers/game_globals_player_control.h"
#include "headers/blam_data_globals.h"


void player_control_initialize_for_new_map(void)
{
    player_control_globals_t *globals = player_control_globals;
    globals->testing_for_action_flags = 0;
    globals->action_test_flags = 0;
    globals->inhibit_button_flags = 0;
    globals->control_flags = 0;

    for ( int16_t p = 0; p < 2; p = (int16_t)(p + 1) )
    {
        player_control *player = &globals->players[p];
        game_globals_player_control *tuning = (game_globals_player_control *)global_game_globals->player_control.address;

        memset(player, 0, sizeof(player_control));
        player->pitch_maximum = 1.4922565f;
        player->unit_index = -1;
        player->pitch_minimum = -1.4922565f;
        player->desired_weapon_index = -1;
        player->desired_grenade_index = -1;
        player->desired_zoom_level = -1;
        player->use_autolevel = 0;
        player->target_object_index = -1;
        player->inhibited_button_bit_vector = 0;
        player->reset_button_when_released_bit_vector = 0;

        if ( player_look_pitch_rate[p] == 0.0f )
            player_look_pitch_rate[p] = tuning->look_default_pitch_rate;
        if ( player_look_yaw_rate[p] == 0.0f )
            player_look_yaw_rate[p] = tuning->look_default_yaw_rate;
    }
}
