/* vehicle_remapper_init @0x83700DC8 — reset the multiplayer vehicle remapper for a fresh map: per team and per
 * vehicle type, clear the spawn bookkeeping, seed max_to_spawn from the active game variant's vehicle set, and
 * record the multiplayer info reference index for each vehicle type.
 *
 * Deviation: the decompiler walks the vehicles[][] array with raw byte pointers (strides 80/160); reproduced
 * here with equivalent struct indexing — outer loop = team, inner loop = vehicle type — for legibility. */

#include <stdint.h>
#include "headers/vehicle_remapper_s.h"
#include "headers/game_globals_multiplayer_information.h"
#include "headers/game_globals_tag.h"
#include "headers/game_variant.h"
#include "headers/tag_reference.h"
#include "headers/multiplayer_vehicle_index.h"

extern game_variant *game_engine_get_variant(void);
extern uint8_t game_engine_running(void);
extern int vehicle_remapper_get_spawn_count(int team, unsigned int vehicle_type);

void vehicle_remapper_init(void)
{
    game_globals_tag *globals = global_game_globals;
    game_variant *variant = game_engine_get_variant();

    vehicle_remapper.processed = 0;
    vehicle_remapper.is_teamplay = (variant && variant->universal_variant.teams) ? 1 : 0;

    for ( int team = 0; team < 2; ++team )
    {
        for ( int vehicle_type = 0; vehicle_type < MULTIPLAYER_MAX_VEHICLES; ++vehicle_type )
        {
            vehicle_team_spawn_info_s *info = &vehicle_remapper.vehicles[vehicle_type][team];
            info->map_count = 0;
            info->num_spawned = 0;
            info->max_to_spawn = vehicle_remapper_get_spawn_count(team, vehicle_type);
            for ( int slot = 0; slot < 8; ++slot )
            {
                info->team_vehicles[slot].scenario_datum_index = -1;
                info->team_vehicles[slot].spawn = 0;
            }
            if ( game_engine_running() )
                /* was raw `*(u32*)(vehicles.address + 16*type + 12)` — tag_reference slot .index */
                info->ref_index = ((tag_reference *)((game_globals_multiplayer_information *)globals->multiplayer_information.address)->vehicles.address)[vehicle_type].index;
            else
                info->ref_index = -1;
        }
    }
}
