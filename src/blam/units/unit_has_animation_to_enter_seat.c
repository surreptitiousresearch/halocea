/* unit_has_animation_to_enter_seat @0x836D11E0 — test whether a unit has an entry animation for a given
 * seat of a vehicle. Returns 1 if the seat index is in range and either the unit's object permits any seat
 * (object type field at +180 == 1) or the unit's animation graph has a matching seat label; 0 otherwise. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/vehicle_definition.h"
#include "headers/unit_seat.h"
#include "headers/global_tag_instances.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

extern uint8_t unit_set_or_test_seat_and_weapon_label(int object_index, const char *seat_label, const char *weapon_label, uint8_t change_flag);

uint8_t unit_has_animation_to_enter_seat(int unit_index, int vehicle_index, int16_t seat_index)
{
    object_datum *vehicle_object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum;
    vehicle_definition *definition = TAG_GET(vehicle_definition, vehicle_object->definition_index);

    if ( seat_index < 0 || seat_index >= definition->unit.seats.count )
        return 0;

    object_datum *unit_object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    if ( unit_object->object.type == object_type_vehicle )
        return 1;

    const char *seat_label = ((unit_seat *)definition->unit.seats.address)[seat_index].label;
    if ( !unit_set_or_test_seat_and_weapon_label(unit_index, seat_label, nullptr, 0) )
        return 0;
    return 1;
}
