/* game_engine_infinite_grenades @0x83748458 — does the active game variant grant the given player
 * unlimited grenades? True only when a game engine is running, the player is valid, the engine isn't in
 * its teardown phase (flags bit 0x4), and the variant's "infinite grenades" universal flag (bit 0x2) is set. */

/* blam_data_globals.h first: supplies wchar_t before dependent headers are parsed (migration) */
#include "headers/blam_data_globals.h"
#include "headers/game_engine.h"
#include "headers/game_engine_flags.h"
#include "headers/game_engine_globals.h"
#include "headers/game_variant.h"
#include "headers/game_engine_universal_variant_flags.h"

/* game_engine: canonical decl from game_engine.h (via blam_data_globals.h) */
#include <stdint.h>

extern unsigned int game_engine_infinite_grenades_internal(void);

uint8_t game_engine_infinite_grenades(int player_index)
{
    /* DEVIATION: collapsed inlined copy of game_engine_infinite_grenades_internal@0x83748428 (zero-xref) to its call; the callee itself performs the 5-or-more-players (flags bit 0x4) test and the infinite-grenades bit read (disasm-confirmed @0x8374843C/0x8374844C), so only this caller's game_engine/player_index preconditions remain outside the call. */
    if (game_engine && player_index != -1)
        return game_engine_infinite_grenades_internal();
    return 0;
}
