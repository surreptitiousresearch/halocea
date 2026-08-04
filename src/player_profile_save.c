/* player_profile_save @0x83765AE8 — persist a player profile by index, unless it is the default profile
 * (index -1), in which case it is not written. */

#include <stdint.h>
#include "headers/player_profile.h"

extern void console_printf_only_if_open(uint8_t clear, const char *format, ...);
extern int player_profile_write(int index, player_profile *profile);

void player_profile_save(int player_profile_index, player_profile *profile)
{
    if ( player_profile_index == -1 )
        console_printf_only_if_open(0, "profile not saved since it was a default profile");
    else
        player_profile_write(player_profile_index, profile);
}
