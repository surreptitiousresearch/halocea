/* playlist_profiles_enumerate_available_to_local_player_index @0x83782630 — lazily creates the default
 * on-disk playlist profiles the first time this is called, then enumerates them (saved_game_file_type 1 =
 * playlist profiles, always including defaults). */
#include <stdint.h>
#include "headers/saved_game_file_type.h"

extern int first_time_0;

extern void playlist_profile_create_default_profiles_on_disk(void);
extern void saved_game_files_enumerate_available_to_local_player_index(int16_t player_index, uint16_t saved_game_file_type, uint16_t *number_of_profiles, int *player_profile_indices, uint8_t include_default_profiles);

void playlist_profiles_enumerate_available_to_local_player_index(int16_t local_player_index, uint16_t *number_of_profiles, int *playlist_profile_indices)
{
    if (first_time_0 == 1)
    {
        playlist_profile_create_default_profiles_on_disk();
        first_time_0 = 0;
    }

    saved_game_files_enumerate_available_to_local_player_index(
        local_player_index, _saved_game_file_type_play_list, number_of_profiles, playlist_profile_indices, 1);
}
