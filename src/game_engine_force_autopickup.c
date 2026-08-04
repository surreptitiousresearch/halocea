/* game_engine_force_autopickup @0x83748458 — game-mode override that forces a unit to auto-pick a weapon.
 * Only Capture the Flag (game_engine_index 1) forces it, and only for the flag "weapon" itself, so a player
 * walking over the flag always picks it up. unit_index is unused. */

/* blam_data_globals.h first: supplies wchar_t before dependent headers are parsed (migration) */
#include <stdint.h>
#include "headers/blam_data_globals.h"
#include "headers/game_variant.h"
#include "headers/game_engine_type.h"

extern uint8_t weapon_is_flag(int weapon_index);

uint8_t game_engine_force_autopickup(int unit_index, int weapon_index)
{
    if (!game_engine)
        return 0;
    if (global_variant.game_engine_index != game_engine_ctf) /* CTF */
        return 0;
    return weapon_is_flag(weapon_index) != 0;
}
