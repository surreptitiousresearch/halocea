/* copy_relevant_profile_data @0x83802748 — for `type == 2` (name/appearance sync?), copies the player's
 * name and primary color from `profile` into `dst`, and marks every single-player map's low nibble as
 * "seen" (bit 0xF) in `dst->single_player_map_flags`. Always reports success. */

#include <stdint.h>
#include <string.h>
#include "headers/player_profile.h"

uint8_t copy_relevant_profile_data(int type, player_profile *dst, const player_profile *profile)
{
    if ( type == 2 )
    {
        memcpy(dst->player_name, profile->player_name, sizeof(dst->player_name));
        dst->player_name[11] = 0;
        dst->primary_color_index = profile->primary_color_index;

        for ( int i = 0; i < 10; ++i )
            dst->single_player_map_flags[i] |= 0xF;
    }

    return 1;
}
