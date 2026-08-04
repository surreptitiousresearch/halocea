/* game_engine_has_teams @0x837484A0 — is the active game engine a team game? Returns the variant's
 * teams flag when a game engine is running, otherwise 0. */

/* blam_data_globals.h first: supplies wchar_t before dependent headers are parsed (migration) */
#include "headers/blam_data_globals.h"
#include "headers/game_variant.h"
#include <stdint.h>


uint8_t game_engine_has_teams(void)
{
    if ( game_engine )
        return global_variant.universal_variant.teams;
    return 0;
}
