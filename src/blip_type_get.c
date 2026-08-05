/* blip_type_get @0x837BC758 — classify a motion-sensor blip's icon type for a given local player:
 * 5 = no object, 0 = the object is the local player's own controlled unit, 2 = not a verified
 * unit/vehicle-type object, 3/4 = ally/enemy vehicle occupied by a driver or gunner (4 if the vehicle
 * has no seat occupant AND its tag name starts with "c_dropship" — otherwise 3 when unoccupied), 1/2 =
 * ally/enemy for a non-"type 2" object.
 *
 * DEVIATION: the `_cntlzw(x) & 0x20 == 0` idioms are the standard "x != 0" boolean-from-count-leading-
 * zeros pattern (cntlzw returns 32, the 0x20 bit, only when x == 0); simplified accordingly. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/object_datum.h"
#include "headers/vehicle_datum.h"
#include "headers/vehicle_definition.h"
#include "headers/unit_seat.h"
#include "headers/object_type.h"
#include "headers/blip_type.h"
#include "headers/blam_data_globals.h"


extern int local_player_get_player_index(int16_t local_player_index);
extern int player_index_from_unit_index(int unit_index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern uint8_t game_team_is_enemy(int16_t our_team, int16_t other_team);
extern int strncmp(const char *str1, const char *str2, unsigned int max_count);

uint8_t blip_type_get(int object_index, int local_player_index)
{
    int local_player_team = DATA_ARRAY_ELEMENT(player_data, player_datum,
        local_player_get_player_index(local_player_index))->team_index;

    if (object_index == -1)
        return _blip_type_custom;

    int controlling_player_index;
    int owner_player_index = player_index_from_unit_index(object_index);
    if (owner_player_index == -1)
        controlling_player_index = -1;
    else
        controlling_player_index = DATA_ARRAY_ELEMENT(player_data, player_datum,
            player_index_from_unit_index(object_index))->local_player_index;

    if (controlling_player_index == local_player_index)
        return _blip_type_self;

    if (!object_try_and_get_and_verify_type(object_index, object_mask_unit))
        return _blip_type_enemy;


    if (object_try_and_get_and_verify_type(object_index, object_mask_vehicle))
    {
        vehicle_datum *vehicle = (vehicle_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
        int seat_unit_index = vehicle->unit.gunner_object_index;
        if (seat_unit_index == -1 && (seat_unit_index = vehicle->unit.driver_object_index) == -1)
        {
            vehicle_definition *tag_definition = TAG_GET(vehicle_definition, vehicle->definition_index);
            if (tag_definition->unit.seats.count > 1
                && !strncmp(((unit_seat *)tag_definition->unit.seats.address)[0].label, "c_dropship", 0xAu))
                return _blip_type_vehicle_enemy;
            return _blip_type_vehicle_friend;
        }

        int16_t seat_team = (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, seat_unit_index)->datum)->object.owner_team_index;
        return (game_team_is_enemy(seat_team, local_player_team) != 0) + _blip_type_vehicle_friend;
    }

    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    return (game_team_is_enemy(object->object.owner_team_index, local_player_team) != 0) + _blip_type_friend;
}
