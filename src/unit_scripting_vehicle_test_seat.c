/* unit_scripting_vehicle_test_seat @0x836C9FD0 — return whether a specific child unit is occupying the named
 * seat of the given vehicle. Walks the vehicle definition's seat block (count +0x2E4, block +0x2E8, stride
 * 284, seat name at +4) for a name match, then verifies the child's parent (+0x11C) is the vehicle and its
 * seat index (+0x2F0) is that seat. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/vehicle_definition.h"
#include "headers/unit_seat.h"
#include "headers/blam_data_globals.h"

extern int stricmp(const char *a, const char *b);

uint8_t unit_scripting_vehicle_test_seat(int unit_index, const char *seat_name, int child_index)
{
    if ( unit_index == -1 || child_index == -1 )
        return 0;

    object_datum *unit = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    vehicle_definition *definition = TAG_GET(vehicle_definition, unit->definition_index);
    int seat_count = definition->unit.seats.count;
    if ( seat_count <= 0 )
        return 0;

    unit_seat *seats = (unit_seat *)definition->unit.seats.address;
    for ( int seat = 0; seat < seat_count; seat = (__int16)(seat + 1) )
    {
        const char *seat_definition_name = seats[seat].label;
        if ( !stricmp(seat_name, seat_definition_name) )
        {
            unit_datum *child = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, child_index)->datum;
            if ( child->object.parent_object_index == unit_index && child->unit.parent_seat_index == seat )
                return 1;
        }
    }
    return 0;
}
