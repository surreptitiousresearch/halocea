/* blam_data_globals.h first: supplies wchar_t before dependent headers are parsed (migration) */
#include <stdint.h>
#include "headers/blam_data_globals.h"
#include "headers/game_variant.h"
#include "headers/game_engine_type.h"
#include "headers/game_engine_universal_variant_flags.h"

/* game_engine: canonical decl from game_engine.h (via blam_data_globals.h) */

uint8_t game_engine_hud_draw_motion_sensor(int player_index)
{
    if ( !game_engine )
        return 1;

    uint8_t result = global_variant.universal_variant.goal_radar == 0;
    if ( global_variant.game_engine_index == game_engine_slayer )
        result &= (global_variant.game_engine_variant.ctf.flag_must_reset == 0) - 1;
    return result | ((global_variant.universal_variant.flags >> _allow_motion_tracker_bit) & 1);
}
