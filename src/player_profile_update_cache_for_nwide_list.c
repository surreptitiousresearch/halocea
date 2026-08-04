/* player_profile_update_cache_for_nwide_list @0x83780398 — reconcile the 3-slot
 * cached_player_profile cache against a requested list of profile indices for the n-wide multiplayer
 * settings list: mark which existing cache slots are still wanted, then for each requested profile not
 * already cached, load it into the first free (unwanted) slot. Sibling of
 * variant_profile_update_cache_for_nwide_list.c, which does the same thing for cached_variant_profile.
 *
 * DEVIATION: the loop bound "(int)v3 < (int)cached_variant_profile" in the decompile is a pointer
 * comparison against the next global in memory, not an explicit count — cached_player_profile is a
 * fixed 3-element array (per DB layout), reproduced here as a plain bounded loop. */

#include <stdint.h>
#include "headers/cached_player_profile.h"

#include "headers/player_profile.h"
extern uint8_t player_profile_get(int player_profile_index, player_profile *profile);

void player_profile_update_cache_for_nwide_list(int *profile_index, int number_of_profile_indices)
{
    unsigned __int8 still_wanted[3] = { 0, 0, 0 };

    for ( int slot = 0; slot < 3; ++slot )
    {
        if ( cached_player_profile[slot].profile_index == -1 )
            continue;

        for ( int requested = 0; requested < number_of_profile_indices; ++requested )
        {
            if ( cached_player_profile[slot].profile_index == profile_index[requested] )
            {
                still_wanted[slot] = 1;
                break;
            }
        }
    }

    for ( int requested = 0; requested < number_of_profile_indices; ++requested )
    {
        int requested_profile_index = profile_index[requested];
        if ( requested_profile_index == -1 )
            continue;

        int found_slot = 3;
        for ( int slot = 0; slot < 3; ++slot )
        {
            if ( cached_player_profile[slot].profile_index == requested_profile_index )
            {
                found_slot = slot;
                break;
            }
        }

        if ( found_slot == 3 )
        {
            int free_slot;
            for ( free_slot = 0; free_slot < 3; ++free_slot )
            {
                if ( still_wanted[free_slot] != 1 )
                    break;
            }

            if ( player_profile_get(requested_profile_index, &cached_player_profile[free_slot].profile) )
            {
                still_wanted[free_slot] = 1;
                cached_player_profile[free_slot].profile_index = requested_profile_index;
            }
        }
    }
}
