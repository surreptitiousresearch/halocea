/* game_variant_build_vehicle_set @0x837495D0 — map a vehicle-set preset index (game_engine_vehicles, 0..7) to
 * its packed vehicle-set bitfield value; out-of-range presets return 0x89249000 (preset nibble =
 * _game_engine_vehicles_custom over the default vehicle bits).
 * ATTESTATION FIX 2026-07-28: four constants corrected against disasm (0x837495D0..0x83749654):
 * default 0x09249000 (was 0x09240000), banshee 0x60008000 (was 0x60020000), turrets 0x70001000
 * (was 0x70020000), out-of-range 0x89249000 (was 0x89000000). */

#include <stdint.h>
#include "headers/game_engine_vehicles.h"

uint32_t game_variant_build_vehicle_set(unsigned int preset)
{
    if ( preset > 7 )
        return 0x89249000u;
    switch ( preset )
    {
        case _game_engine_vehicles_default:  return 0x09249000u;
        case _game_engine_vehicles_none:     return 0x10000000u;
        case _game_engine_vehicles_warthog:  return 0x28000000u;
        case _game_engine_vehicles_ghost:    return 0x31000000u;
        case _game_engine_vehicles_tank:     return 0x40200000u;
        case _game_engine_vehicles_rwarthog: return 0x50040000u;
        case _game_engine_vehicles_banshee:  return 0x60008000u;
    }
    return 0x70001000u; /* _game_engine_vehicles_turrets */
}
