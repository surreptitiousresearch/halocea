/* map_list_is_map_original_xbox_mp_map @0x8376720C — whether the builtin map at the given index shipped as an
 * original-Xbox multiplayer map. Returns 0 for out-of-range indices (>= 19). */

#include <stdint.h>
#include "headers/multiplayer_map_s.h"
#include "headers/blam_data_globals.h"


uint8_t map_list_is_map_original_xbox_mp_map(int index)
{
    if ( index < 19 )
        return builtin_multiplayer_maps[index].original_xbox_map;
    return 0;
}
