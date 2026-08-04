/* reset_local_player_profile @0x83699830 — reset one local player's UI profile slot to the
 * default profile and clear its bookkeeping fields, then push the default controls to the
 * input layer.
 *
 * The decompiler expressed the per-player slot with raw byte offsets (1988 * index); the DB
 * struct layout resolves these to player_ui_globals.player_data[index].profile /
 * .profile_index and single_player_controller_index[index]. */

#include <stdint.h>
#include "headers/player_ui_globals.h"

#include "headers/player_profile.h"
extern void player_profile_build_default_profile(player_profile *profile, int player_index, uint8_t try_inherit);
extern void set_local_player_controls_from_player_profile(int16_t local_player_index);

void reset_local_player_profile(int16_t local_player_index)
{
    player_profile_build_default_profile(&player_ui_globals.player_data[local_player_index].profile, 0, 0);
    player_ui_globals.player_data[local_player_index].profile_index = -1;
    player_ui_globals.single_player_controller_index[local_player_index] = -1;
    set_local_player_controls_from_player_profile(local_player_index);
}
