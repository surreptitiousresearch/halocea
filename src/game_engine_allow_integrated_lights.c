/* game_engine_allow_integrated_lights @0x8374A2E0 — decide whether an object may show its integrated lights
 * (e.g. flashlight glow). Always allowed when no game engine is active. Disallowed when the variant forbids
 * it (game_engine_globals.flags bit 0x2) or effects are off (rasterizer_effects_level <= 0). At the highest
 * effects level (>= 2) it's allowed unconditionally; at the intermediate level it's allowed only for units
 * controlled by a local player.
 * DEVIATION: the local-player test's arithmetic (v4+1-(v4+(v4==-1))) is reproduced verbatim; it evaluates to
 * 1 when the unit has a local player (index != -1), else 0. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/game_engine_flags.h"
#include "headers/player_datum.h"
#include "headers/game_engine.h"
#include "headers/game_engine_globals.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/blam_data_globals.h"

extern int player_index_from_unit_index(int unit_index);

int game_engine_allow_integrated_lights(int object_index)
{
    if ( !game_engine )
        return 1;

    unsigned char allowed;
    if ( (game_engine_globals.flags & (1u << _game_engine_disable_integrated_lights_bit)) != 0 || (allowed = 1, rasterizer_debug_options.rasterizer_effects_level <= 0) )
        allowed = 0;
    if ( !allowed || rasterizer_debug_options.rasterizer_effects_level >= 2 )
        return allowed;

    int player_index = player_index_from_unit_index(object_index);
    if ( player_index != -1 )
    {
        int16_t local_player_index = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->local_player_index;
        return local_player_index + 1 - (local_player_index + (local_player_index == -1));
    }
    return 0;
}
