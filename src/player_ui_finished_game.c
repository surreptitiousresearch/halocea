/* player_ui_finished_game @0x83699230 — mark player 1's profile as having finished the game (profile flag bit
 * 2) and save it if it is a real (non-default) profile.
 *
 * player_data[0].profile.flags (@284) bit 2 = finished game; player_data[0].profile_index (@1980). */

#include "headers/player_ui_globals.h"
#include "headers/button_preset_flags.h"

extern void player_profile_save(int player_profile_index, player_profile *profile);

void player_ui_finished_game(void)
{
    player_ui_globals.player_data[0].profile.flags |= (1u << _profile_finished_game_bit);
    int profile_index = player_ui_globals.player_data[0].profile_index;
    if ( profile_index != -1 )
        player_profile_save(profile_index, &player_ui_globals.player_data[0].profile);
}
