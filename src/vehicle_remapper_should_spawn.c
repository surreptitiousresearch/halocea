/* vehicle_remapper_should_spawn @0x83701470 — decide whether a given scenario vehicle placement should spawn
 * under the active variant's vehicle set. Outside a running game engine every vehicle spawns. Otherwise the
 * placement's ref index is matched to a vehicle type, then the per-team spawn lists are scanned for the
 * scenario datum index; its recorded spawn flag is returned.
 *
 * Deviation: raw-pointer ref_index walk (stride 2 structs) and flat team_vehicles[] indexing reproduced with
 * struct indexing, matching vehicle_remapper_add_vehicle. */

#include <stdint.h>
#include "headers/vehicle_remapper_s.h"

extern uint8_t game_engine_running(void);

extern int vehicle_remapper_get_type(int vehicle_ref_index);
uint8_t vehicle_remapper_should_spawn(int vehicle_ref_index, unsigned int scenario_datum_index)
{
    if ( !game_engine_running() )
        return 1;

    /* DEVIATION: vehicle_remapper_get_type@0x83700D80 inlined here (linear ref_index search identical to add_vehicle's copy); collapsed to a call — donor's int -1 return sign-extends into the unsigned vehicle_type, preserving the disasm's `v4 <= 5` unsigned-compare semantics below. */
    unsigned int vehicle_type = (unsigned int)vehicle_remapper_get_type(vehicle_ref_index);

    int result = 0;
    if ( vehicle_type <= 5 )
    {
        char found = 0;
        for ( int team = 0; team < 3 && !found; ++team )
        {
            unsigned int flat_slot = 2 * vehicle_type + team;
            unsigned int map_count = vehicle_remapper.vehicles[0][flat_slot].map_count;
            if ( map_count )
            {
                unsigned int base = 10 * flat_slot;
                for ( unsigned int i = 0; i < map_count; ++i )
                {
                    unsigned int entry = base + i;
                    if ( vehicle_remapper.vehicles[0][0].team_vehicles[entry].scenario_datum_index == scenario_datum_index )
                    {
                        found = 1;
                        result = vehicle_remapper.vehicles[0][0].team_vehicles[entry].spawn;
                        break;
                    }
                }
            }
        }
    }
    return result;
}
