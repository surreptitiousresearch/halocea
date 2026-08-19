/* game_engine_infinite_grenades_internal @0x83748428 — true when equipment randomization is disabled
 * (game_engine_globals.flags bit2 clear) and the active variant's "infinite grenades" bit is set. */

/* blam_data_globals.h first: supplies wchar_t before dependent headers are parsed (migration) */
#include "headers/blam_data_globals.h"
#include "headers/game_engine_globals.h"
#include "headers/game_engine_flags.h"
#include "headers/game_variant.h"
#include "headers/game_engine_universal_variant_flags.h"


unsigned int game_engine_infinite_grenades_internal(void)
{
    if ((game_engine_globals.flags & (1u << _game_engine_5_or_more_players_bit)) == 0)
        return (global_variant.universal_variant.flags >> _infinite_grenades_bit) & 1;
    return 0;
}
