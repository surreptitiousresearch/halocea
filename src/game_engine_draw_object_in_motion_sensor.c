/* game_engine_draw_object_in_motion_sensor @0x8374A268 */
/* blam_data_globals.h first: supplies wchar_t before dependent headers are parsed (migration) */
#include <stdint.h>
#include "headers/blam_data_globals.h"
#include "headers/game_variant.h"
#include "headers/game_engine_universal_variant_flags.h"


uint8_t game_engine_draw_object_in_motion_sensor(int unit_index)
{
    if (game_engine)
        return (global_variant.universal_variant.flags >> _allow_motion_tracker_bit) & 1;
    return 1;
}
