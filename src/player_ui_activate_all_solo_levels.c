/* player_ui_activate_all_solo_levels @0x836991DC — debug/unlock: enable all single-player levels in
 * local player 0's profile (unlock all 10 map flags + the "campaign complete" profile flag), then
 * persist the profile.
 *
 * Fields reached through the DB-verified _player_data / player_profile layout:
 *   player_profile.single_player_map_flags[10] (+0x11E), player_profile.flags (+0x11C),
 *   _player_data.profile_index (+0x7BC). */

#include "headers/player_ui_globals.h"
#include "headers/button_preset_flags.h"  /* profile.flags bits: _profile_*_bit */

extern void player_profile_save(int player_profile_index, player_profile *profile);

void player_ui_activate_all_solo_levels(void)
{
    _player_data *player_data = &player_ui_globals.player_data[0];

    /* recovered: profile[0x11E + i] |= 0xF -> profile.single_player_map_flags[i] */
    for ( int map_index = 0; map_index < 10; ++map_index )
        player_data->profile.single_player_map_flags[map_index] |= 0xF;
    /* recovered: *(u16*)(profile+0x11C) |= 4u -> profile.flags bit2 = campaign complete */
    player_data->profile.flags |= (1u << _profile_finished_game_bit);

    /* recovered: *(int*)(profile+0x7BC) -> _player_data.profile_index */
    int profile_index = player_data->profile_index;
    if ( profile_index != -1 )
        /* recovered: (player_profile*)&player_ui_globals -> &player_data[0].profile (offset 0) */
        player_profile_save(profile_index, &player_data->profile);
}
