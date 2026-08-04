/* vehicle_remapper_get_spawn_count @0x83700CB0 — number of vehicles of a given type the active variant allows
 * to spawn for a team. The variant stores a packed vehicle-set value (or a preset index in the top nibble,
 * expanded via game_variant_build_vehicle_set); each vehicle type occupies a 3-bit count field. */

#include <stdint.h>
#include "headers/game_variant.h"
#include "headers/vehicle_remapper_vehicle_type.h"
#include "headers/game_engine_vehicles.h"

extern game_variant *game_engine_get_variant(void);
extern uint32_t game_variant_build_vehicle_set(unsigned int preset);

int vehicle_remapper_get_spawn_count(int team, unsigned int vehicle_type)
{
    int count = 0;
    game_variant *variant = game_engine_get_variant();
    if ( variant )
    {
        vehicle_options_s vehicle_set = (team == 1) ? variant->universal_variant.vehicle_set2
                                                    : variant->universal_variant.vehicle_set;
        /* a preset (not custom) is expanded into a literal per-type count bitfield first */
        if ( vehicle_set.__s1.preset != _game_engine_vehicles_custom )
            vehicle_set.value = game_variant_build_vehicle_set(vehicle_set.__s1.preset);
        if ( vehicle_type <= 5 )
        {
            switch ( vehicle_type )
            {
                case _vehicle_remapper_warthog:   return vehicle_set.__s1.warthogs;
                case _vehicle_remapper_ghost:     return vehicle_set.__s1.ghosts;
                case _vehicle_remapper_scorpion:  return vehicle_set.__s1.scorpions;
                case _vehicle_remapper_banshee:   return vehicle_set.__s1.banshees;
                case _vehicle_remapper_cg_turret: return vehicle_set.__s1.cg_turrets;
            }
            return vehicle_set.__s1.rwarthogs;
        }
    }
    return count;
}
