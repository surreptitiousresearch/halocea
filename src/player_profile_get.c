/* player_profile_get @0x83764E20 — fetch a player profile by index. For the default profile (index -1) copy
 * the global default profile and report "was default" (returns 0); otherwise read the stored profile. */

#include <stdint.h>
#include <string.h>
#include "headers/player_profile.h"
#include "headers/player_profile_globals.h"

extern uint8_t player_profile_read(int index, player_profile *profile);

uint8_t player_profile_get(int player_profile_index, player_profile *profile)
{
    if ( player_profile_index != -1 )
        return player_profile_read(player_profile_index, profile);

    /* recovered: &player_profile_globals -> &player_profile_globals.default_profile */
    memcpy(profile, &player_profile_globals.default_profile, sizeof(player_profile));
    return 0;
}
