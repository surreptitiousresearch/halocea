/* vehicle_remapper_process_by_teams @0x83701238 — for a team game, decide which registered scenario vehicles
 * actually spawn for each team. If the team's variant vehicle-set value carries a literal set (high nibble 0),
 * every placement whose engine-specific spawn-flag bit is set spawns; otherwise the first max_to_spawn
 * placements of each type spawn (counted into num_spawned).
 *
 * Deviation: flat team_vehicles[] indexing (10 * flat_slot + entry) reproduced from the decompiler's pointer
 * arithmetic, spanning whole 80-byte structs. */

#include "headers/vehicle_remapper_s.h"
#include "headers/game_variant.h"
#include "headers/game_engine_vehicles.h"
#include "headers/game_engine_type.h"
#include "headers/multiplayer_spawn_game_type.h"

extern game_variant *game_engine_get_variant(void);

/* Attested void: no r3 is defined on any return path; the sole caller ignores the result. The local
 * `variant` only threads game_engine_get_variant() results within the loops. */
void vehicle_remapper_process_by_teams(void)
{
    game_variant *variant = 0;
    for ( int team = 0; team <= 1; ++team )
    {
        for ( int vehicle_type = 0; vehicle_type < 6; ++vehicle_type )
        {
            /* DEVIATION: the flat slot 2*vehicle_type+team and the 10-unit team_vehicles stride were folded
             * through vehicles[0][0]; vehicles is [6][2] of 80 bytes and team_vehicles[8] sits at +16, so
             * [vehicle_type][team].team_vehicles[j] is the same address for every j the loop reaches. */
            int map_count = vehicle_remapper.vehicles[vehicle_type][team].map_count;

            char literal_set = 0;
            variant = game_engine_get_variant();
            if ( variant )
            {
                vehicle_options_s vehicle_set = (team == 1) ? variant->universal_variant.vehicle_set2
                                                            : variant->universal_variant.vehicle_set;
                if ( vehicle_set.__s1.preset == _game_engine_vehicles_default )
                    literal_set = 1;
            }

            if ( literal_set )
            {
                for ( int j = 0; j < map_count; ++j )
                {
                    vehicle_info_s *vehicle = &vehicle_remapper.vehicles[vehicle_type][team].team_vehicles[j];
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
                        vehicle_remapper.vehicles[vehicle_type][team].team_vehicles[j].spawn = 1;
                }
            }
            else
            {
                int max_to_spawn = vehicle_remapper.vehicles[vehicle_type][team].max_to_spawn;
                for ( int k = 0; k < map_count && max_to_spawn > 0; ++k )
                {
                    --max_to_spawn;
                    ++vehicle_remapper.vehicles[vehicle_type][team].num_spawned;
                    vehicle_remapper.vehicles[vehicle_type][team].team_vehicles[k].spawn = 1;
                }
            }
        }
    }
}
