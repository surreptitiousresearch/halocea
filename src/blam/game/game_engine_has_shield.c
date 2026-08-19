/* game_engine_has_shield @0x83748510 — does the given player's unit have a shield in the current game?
 * In single-player (no game engine) or for an invalid player, the answer is always yes. In a multiplayer
 * game the variant's universal flags decide it: shields are present unless flag 0x8 ("no shields") is set
 * — i.e. the inverted bit 3 of the flags. */

/* blam_data_globals.h first: supplies wchar_t before dependent headers are parsed (migration) */
#include <stdint.h>
#include "headers/blam_data_globals.h"
#include "headers/game_engine.h"
#include "headers/game_variant.h"
#include "headers/game_engine_universal_variant_flags.h"


uint8_t game_engine_has_shield(int player_index)
{
    if (game_engine && player_index != -1)
        return (~global_variant.universal_variant.flags >> _no_shields_bit) & 1;
    return 1;
}
