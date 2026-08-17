/* generate_default_player_profile @0x83699330 */
#include <stdint.h>
#include "headers/player_profile.h"

extern void player_profile_build_default_profile(player_profile *profile, int player_index, uint8_t try_inherit);

void generate_default_player_profile(player_profile *profile)
{
    player_profile_build_default_profile(profile, 0, 0);
}
