/* unit_seat_allow_noncombatants @0x836CAE68 — true if the given seat of a unit (vehicle) definition
 * permits noncombatants (unit_seat.flags bit 0xA). Resolves the object's unit/vehicle tag definition
 * and indexes its shared seats block. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/unit_definition.h"
#include "headers/unit_seat.h"
#include "headers/unit_seat_flags.h"
#include "headers/blam_data_globals.h"


uint8_t unit_seat_allow_noncombatants(int vehicle_index, int16_t seat_index)
{
    object_datum *object =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum;
    unit_definition *definition = TAG_GET(unit_definition, object->definition_index);

    if ( seat_index >= 0 && seat_index < definition->unit.seats.count )
    {
        unit_seat *seat = &((unit_seat *)definition->unit.seats.address)[seat_index];
        return (seat->flags >> _unit_seat_allow_noncombatants_bit) & 1;
    }
    return 0;
}
