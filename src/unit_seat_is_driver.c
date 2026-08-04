/* unit_seat_is_driver @0x836CADA4 — true if the given seat of a vehicle is its driver seat. Reads the
 * vehicle object's definition tag, bounds-checks the seat index against the seat block count and returns
 * the seat's driver flag (bit 0x4). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/unit_seat.h"
#include "headers/unit_seat_flags.h"
#include "headers/blam_data_globals.h"


uint8_t unit_seat_is_driver(int vehicle_index, int16_t seat_index)
{
    unit_datum *vehicle = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum;
    unit_definition *vehicle_definition = TAG_GET(unit_definition, vehicle->definition_index);
    if ( seat_index >= 0 && seat_index < vehicle_definition->unit.seats.count )
        return (((unit_seat *)vehicle_definition->unit.seats.address)[seat_index].flags >> _unit_seat_is_driver_bit) & 1;
    return 0;
}
