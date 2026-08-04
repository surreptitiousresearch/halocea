/* local_player_is_piloting_aircraft @0x837254F8 — is the local player (by controller index) currently
 * seated in a parent vehicle whose tag type is _vehicle_type_human_plane (3) or _vehicle_type_alien_fighter
 * (5), in a seat whose flags have _unit_seat_is_driver_bit (0x4) set (driver/pilot seat)? */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/player_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/unit_datum.h"
#include "headers/vehicle_definition.h"
#include "headers/unit_seat.h"
#include "headers/unit_seat_flags.h"
#include "headers/vehicle_type.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


extern int local_player_get_player_index(int16_t local_player_index);
extern void *datum_try_and_get(const data_array *data, int index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);

uint8_t local_player_is_piloting_aircraft(int16_t controller_index)
{
    int player_index = local_player_get_player_index(controller_index);
    if ( player_index == -1 )
        return 0;

    player_datum *player = datum_try_and_get(player_data, player_index);
    if ( !player )
        return 0;

    unit_datum *unit = object_try_and_get_and_verify_type(player->unit_index, object_mask_unit);
    if ( !unit )
        return 0;

    int vehicle_index = unit->object.parent_object_index;
    if ( vehicle_index == -1 )
        return 0;

    __int16 seat_index = unit->unit.parent_seat_index;
    if ( seat_index == -1 )
        return 0;

    unit_datum *vehicle = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum);
    vehicle_definition *vehicle_def = TAG_GET(vehicle_definition, vehicle->definition_index);

    if ( vehicle_def->vehicle.type != _vehicle_type_human_plane && vehicle_def->vehicle.type != _vehicle_type_alien_fighter )
        return 0;

    unit_seat *seat = (unit_seat *)vehicle_def->unit.seats.address + seat_index;
    return (seat->flags & (1u << _unit_seat_is_driver_bit)) != 0;
}
