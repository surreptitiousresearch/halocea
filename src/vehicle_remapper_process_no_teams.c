/* vehicle_remapper_process_no_teams @0x83701028 — free-for-all counterpart to vehicle_remapper_process_by_teams.
 * For each vehicle type: if the variant carries a literal vehicle set (high nibble 0), spawn every placement in
 * both columns whose engine-specific spawn-flag bit is set; otherwise distribute max_to_spawn spawns across the
 * two columns, always feeding the column with fewer num_spawned that still has room.
 *
 * Deviation: pointer/byte arithmetic (vehicles + type*160, flat team_vehicles[]) reproduced with struct
 * indexing; balancing branch flattened from the decompiler's goto chain. */

#include "headers/vehicle_remapper_s.h"
#include "headers/game_variant.h"
#include "headers/game_engine_vehicles.h"
#include "headers/game_engine_type.h"
#include "headers/multiplayer_spawn_game_type.h"

extern game_variant *game_engine_get_variant(void);

/* Attested void: the sole caller (vehicle_remapper_process) ignores r3; the value left in r3 at the
 * blr is the loop's last `num_spawned + 1` — incidental residue, not a return. */
void vehicle_remapper_process_no_teams(void)
{
    game_variant *variant = 0;
    for ( int vehicle_type = 0; vehicle_type < 6; ++vehicle_type )
    {
        char literal_set = 0;
        variant = game_engine_get_variant();
        if ( variant )
            literal_set = variant->universal_variant.vehicle_set.__s1.preset == _game_engine_vehicles_default;

        if ( !literal_set )
        {
            int max_to_spawn = vehicle_remapper.vehicles[vehicle_type][0].max_to_spawn;
            while ( max_to_spawn > 0 )
            {
                unsigned int num_spawned0 = vehicle_remapper.vehicles[vehicle_type][0].num_spawned;
                unsigned int num_spawned1 = vehicle_remapper.vehicles[vehicle_type][1].num_spawned;
                unsigned int map_count0 = vehicle_remapper.vehicles[vehicle_type][0].map_count;
                unsigned int map_count1 = vehicle_remapper.vehicles[vehicle_type][1].map_count;

                int column = -1;
                if ( num_spawned0 > num_spawned1 )
                {
                    if ( num_spawned1 < map_count1 )       column = 1;
                    else if ( num_spawned0 < map_count0 )  column = 0;
                }
                else
                {
                    if ( num_spawned0 < map_count0 )       column = 0;
                    else if ( num_spawned1 < map_count1 )  column = 1;
                }
                if ( column < 0 )
                    break;

                int flat_slot = 2 * vehicle_type + column;
                --max_to_spawn;
                unsigned int slot_num_spawned = vehicle_remapper.vehicles[0][flat_slot].num_spawned;
                vehicle_remapper.vehicles[0][flat_slot].num_spawned = slot_num_spawned + 1;
                vehicle_remapper.vehicles[0][flat_slot].team_vehicles[slot_num_spawned].spawn = 1;
            }
            continue;
        }

        for ( int column = 0; column <= 1; ++column )
        {
            int flat_slot = 2 * vehicle_type + column;
            int team_base = 10 * flat_slot;
            int map_count = vehicle_remapper.vehicles[vehicle_type][column].map_count;
            for ( int j = 0; j < map_count; ++j )
            {
                int entry = team_base + j;
                vehicle_info_s *vehicle = &vehicle_remapper.vehicles[0][0].team_vehicles[entry];
                int spawn = 0;
                variant = game_engine_get_variant();
                if ( variant )
                {
                    switch ( variant->game_engine_index )
                    {
                        case game_engine_ctf:     spawn = (vehicle->spawn_flags >> _multiplayer_spawn_ctf_bit) & 1;     break;
                        case game_engine_slayer:  spawn = (vehicle->spawn_flags >> _multiplayer_spawn_slayer_bit) & 1;  break;
                        case game_engine_oddball: spawn = (vehicle->spawn_flags >> _multiplayer_spawn_oddball_bit) & 1; break;
                        case game_engine_king:    spawn = (vehicle->spawn_flags >> _multiplayer_spawn_king_bit) & 1;    break;
                        default: spawn = 0;                              break;
                    }
                }
                if ( spawn )
                    vehicle_remapper.vehicles[0][0].team_vehicles[entry].spawn = 1;
            }
        }
    }
}
