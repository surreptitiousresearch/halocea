/* create_race_vehicle @0x8382A058 — race engine: spawn the next race vehicle for a player, if fewer than
 * 8 have been spawned so far. Finds the netgame "vehicle spawn" flag closest to the player's own unit (or
 * just the first one, if the player has no unit), records it as spawned, and — if the race variant has a
 * vehicle configured for this spawn slot — places it at that flag's position/facing and tags the new
 * vehicle object with the flag it spawned from.
 *
 * DEVIATION: the decompiler's array-append index (`race_globals.spawned_vehicles[v4] = ...`) uses an
 * apparently-unset local; semantically this must be the pre-increment `number_of_spawned_vehicles` (the
 * standard "append at current count, then increment" pattern), reproduced as such. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/player_datum.h"
#include "headers/race_globals.h"
#include "headers/scenario.h"
#include "headers/scenario_netgame_flag.h"
#include "headers/object_placement_data.h"
#include "headers/object_datum.h"
#include "headers/real_point3d.h"
#include "headers/blam_data_globals.h"


extern int find_closest_vehicle(const real_point3d *point, int *ignore_these, int ignore_these_count);
extern int race_get_vehicle_to_spawn(int vehicle_number);
extern void object_placement_data_new(object_placement_data *data, int definition_index, int owner_object_index);
extern real_vector3d *vector3d_from_angle(real_vector3d *vector, float angle);
extern int object_new(object_placement_data *data);
extern void vehicle_set_netgame_flag_index(int vehicle_index, int16_t netgame_flag_index);

void create_race_vehicle(int player_index)
{
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
    int unit_index = player->unit_index;

    object_datum *unit_object = nullptr;
    if ( unit_index != -1 )
        unit_object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;

    if ( race_globals.number_of_spawned_vehicles >= 8 )
        return;

    const real_point3d *search_origin = unit_object ? &unit_object->object.position : nullptr;   /* object+92 = _object_datum.position */
    int netgame_flag_index = find_closest_vehicle(search_origin, race_globals.spawned_vehicles,
        race_globals.number_of_spawned_vehicles);
    if ( netgame_flag_index == -1 )
        return;

    race_globals.spawned_vehicles[race_globals.number_of_spawned_vehicles] = netgame_flag_index;
    ++race_globals.number_of_spawned_vehicles;

    scenario_netgame_flag *netgame_flag =
        &((scenario_netgame_flag *)global_scenario->netgame_flags.address)[netgame_flag_index];
    int vehicle_definition_index = race_get_vehicle_to_spawn(race_globals.number_of_spawned_vehicles - 1);
    if ( vehicle_definition_index == -1 )
        return;

    object_placement_data placement;
    object_placement_data_new(&placement, vehicle_definition_index, -1);
    placement.position.n[0] = netgame_flag->position.n[0];
    placement.position.n[1] = netgame_flag->position.n[1];
    placement.position.n[2] = netgame_flag->position.n[2];
    vector3d_from_angle(&placement.forward, netgame_flag->facing);

    int vehicle_index = object_new(&placement);
    vehicle_set_netgame_flag_index(vehicle_index, netgame_flag_index);
}
