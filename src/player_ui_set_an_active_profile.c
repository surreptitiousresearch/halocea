/* player_ui_set_an_active_profile @0x83699B78 — give local player 0 SOME active profile: prefer the first
 * enumerated saved profile (activated under its real index); failing that, fall back to the first
 * default-inclusive enumeration and activate it under index NONE. */

#include <stdint.h>
#include "headers/player_profile.h"

extern void player_profiles_enumerate_available_to_local_player_index(int16_t local_player_index, uint16_t *number_of_profiles, int *player_profile_indices, uint8_t include_default_profiles);
extern uint8_t player_profile_get(int player_profile_index, player_profile *profile);
extern void player_ui_set_active_player_profile(int16_t local_player_index, int profile_index, player_profile *profile);

void player_ui_set_an_active_profile(void)
{
    unsigned short profile_count;
    int profile_indices[1];
    player_profile profile;

    profile_indices[0] = -1;
    profile_count = 1;
    player_profiles_enumerate_available_to_local_player_index(-1, &profile_count, profile_indices, 0);
    if ( profile_count > 0 && profile_indices[0] != -1 )
    {
        if ( player_profile_get(profile_indices[0], &profile) )
            player_ui_set_active_player_profile(0, profile_indices[0], &profile);
        return;
    }

    profile_count = 1;
    player_profiles_enumerate_available_to_local_player_index(-1, &profile_count, profile_indices, 1u);
    if ( profile_count > 0 && profile_indices[0] != -1 && player_profile_get(profile_indices[0], &profile) )
        player_ui_set_active_player_profile(0, -1, &profile);
}
