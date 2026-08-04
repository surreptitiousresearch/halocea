/* player_ui_begin_editing_profile @0x83699398 — snapshot the saved profile at profile_index into
 * both the original and edit buffers, ready for in-place editing. Player (type 0) profiles copy the
 * whole player_profile; playlist (type 1) profiles copy the 0x98-byte game_variant and clear the
 * quitters/round bits (0x180) from the working copy's universal-variant flags. The edit becomes
 * active only when the load succeeds (profile_index left at -1 otherwise). */

#include <stdint.h>
#include "headers/player_ui_globals.h"
#include "headers/player_profile.h"
#include "headers/game_variant.h"
#include "headers/game_engine_universal_variant_flags.h"

extern uint16_t saved_game_file_get_type(int index);
extern uint8_t player_profile_get(int player_profile_index, player_profile *profile);
extern uint8_t playlist_profile_get(int playlist_profile_index, game_variant *variant);
extern void *memcpy(void *dest, const void *src, unsigned int count);

void player_ui_begin_editing_profile(int profile_index)
{
    unsigned __int16 type;

    player_ui_globals.edit_profile_data.profile_index = -1;
    type = saved_game_file_get_type(profile_index);
    if ( !type )
    {
        if ( !player_profile_get(profile_index, &player_ui_globals.edit_profile_data.original_profile.player_profile) )
            return;
        memcpy(
            &player_ui_globals.edit_profile_data.edit_profile,
            &player_ui_globals.edit_profile_data.original_profile,
            sizeof(player_ui_globals.edit_profile_data.edit_profile));
    }
    else if ( type == 1
        && playlist_profile_get(profile_index, (game_variant *)&player_ui_globals.edit_profile_data.original_profile) )
    {
        memcpy(
            &player_ui_globals.edit_profile_data.edit_profile,
            &player_ui_globals.edit_profile_data.original_profile,
            0x98u);
        player_ui_globals.edit_profile_data.edit_profile.variant.universal_variant.flags =
            player_ui_globals.edit_profile_data.original_profile.variant.universal_variant.flags
                & ~((1u << _game_mode_classic_bit) | (1u << _game_mode_standard_bit));
    }
    else
    {
        return;
    }
    player_ui_globals.edit_profile_data.profile_index = profile_index;
}
