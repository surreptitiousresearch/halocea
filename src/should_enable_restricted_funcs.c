/* should_enable_restricted_funcs @0x837221B8 — true when the active local player's profile has the
 * "restricted funcs" bit (flags bit2) set. */

#include <stdint.h>
#include "headers/player_profile.h"
#include "headers/button_preset_flags.h"

extern void player_ui_get_active_player_profile(int16_t local_player_index, player_profile *profile);

uint8_t should_enable_restricted_funcs(void)
{
    player_profile profile;
    player_ui_get_active_player_profile(0, &profile);
    /* profile.flags bit 2 = _profile_finished_game_bit (DB $38D114EE...); the "restricted funcs"
     * gate is keyed off the finished-game profile flag. */
    return (profile.flags >> _profile_finished_game_bit) & 1;
}
