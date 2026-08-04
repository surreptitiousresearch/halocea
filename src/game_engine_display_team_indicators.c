/* game_engine_display_team_indicators 0x837484CC — true when team indicators should be drawn: a multiplayer
 * game engine is running and the active variant is a team game (allow-friendly-navpoints universal flag
 * set and teams enabled). */

/* blam_data_globals.h first: supplies wchar_t before dependent headers are parsed (migration) */
#include <stdint.h>
#include "headers/blam_data_globals.h"
#include "headers/game_engine.h"
#include "headers/game_variant.h"
#include "headers/game_engine_universal_variant_flags.h"


uint8_t game_engine_display_team_indicators(void)
{
    if ( !game_engine )
        return 0;
    if ( (global_variant.universal_variant.flags & (1u << _allow_friendly_navpoints_bit)) == 0 )
        return 0;
    if ( !global_variant.universal_variant.teams )
        return 0;
    return 1;
}
