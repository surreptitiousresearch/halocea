/* player_profiles_enumerate_available_to_local_player_index @0x83764DF8 — thin wrapper that asks the
 * saved-game-file layer to enumerate the player profiles available to a local player (saved_game_file_type 0
 * = player profiles). */

#include <stdint.h>
#include "headers/saved_game_file_type.h"

extern void saved_game_files_enumerate_available_to_local_player_index(int16_t player_index, uint16_t saved_game_file_type, uint16_t *number_of_profiles, int *player_profile_indices, uint8_t include_default_profiles);

void player_profiles_enumerate_available_to_local_player_index(int16_t local_player_index, uint16_t *number_of_profiles, int *player_profile_indices, uint8_t include_default_profiles)
{
    saved_game_files_enumerate_available_to_local_player_index(
        local_player_index, _saved_game_file_type_player_profile, number_of_profiles,
        player_profile_indices, include_default_profiles);
}
