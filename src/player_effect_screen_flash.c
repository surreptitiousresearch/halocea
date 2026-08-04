/* player_effect_screen_flash @0x8373861C — flash a screen-flash definition for the local player that
 * owns `player_index`. No-op for an invalid player (-1) or one with no local-player slot (player datum
 * +2 == -1). The owning local player's live effect state (player_effect_globals->local_player_effect_data
 * [local_player_index], 236-byte stride) is merged with the definition via player_effect_update_screen_flash
 * at full time scale (1.0).
 *
 * DEVIATION: the DB prototype inflates this to 4 args (phantom `a4`) from the FPR-shadow of the float
 * `scale` (f1). The tail call sets only r3 = effect_state and f2 = 1.0, forwarding the caller's r4
 * (screen_flash) and f1 (scale) — confirmed by disasm. */

#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/player_effect_globals.h"
#include "headers/screen_flash_definition.h"
#include "headers/blam_data_globals.h"
extern void player_effect_update_screen_flash(player_effect_datum *effect_state, const screen_flash_definition *screen_flash, float scale, float time_scale);


void player_effect_screen_flash(int player_index, const screen_flash_definition *screen_flash, float scale)
{
    if (player_index == -1)
        return;

    __int16 local_player_index =
        DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->local_player_index;
    if (local_player_index == -1)
        return;

    player_effect_datum *effect_state = &player_effect_globals->local_player_effect_data[local_player_index];
    player_effect_update_screen_flash(effect_state, screen_flash, scale, 1.0f);
}
