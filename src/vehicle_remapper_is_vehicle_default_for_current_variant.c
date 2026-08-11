/* vehicle_remapper_is_vehicle_default_for_current_variant @0x83700C10 — true if this vehicle placement is
 * flagged as the default vehicle for the currently active game engine variant (one spawn_flags bit per
 * engine: slayer=bit0, ctf=bit1, king=bit2, oddball=bit3 — see multiplayer_spawn_game_type.h). False if
 * there is no active variant, or the engine isn't one of the four recognized indices. */

#include <stdint.h>
#include "headers/vehicle_info_s.h"
#include "headers/game_variant.h"
#include "headers/game_engine_type.h"
#include "headers/multiplayer_spawn_game_type.h"

extern game_variant *game_engine_get_variant(void);

uint8_t vehicle_remapper_is_vehicle_default_for_current_variant(vehicle_info_s *vehicle)
{
    game_variant *variant = game_engine_get_variant();
    if ( !variant )
        return 0;

    switch ( variant->game_engine_index )
    {
        case game_engine_ctf:     return (vehicle->spawn_flags >> _multiplayer_spawn_ctf_bit) & 1;
        case game_engine_slayer:  return (vehicle->spawn_flags >> _multiplayer_spawn_slayer_bit) & 1;
        case game_engine_oddball: return (vehicle->spawn_flags >> _multiplayer_spawn_oddball_bit) & 1;
        case game_engine_king:    return (vehicle->spawn_flags >> _multiplayer_spawn_king_bit) & 1;
        default: return 0;
    }
}
