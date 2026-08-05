/* unit_scripting_vehicle_test_seat_list @0x836C9E70 — return whether the named seat of the given vehicle is
 * occupied by any unit present in the supplied object list. For each matching seat name (vehicle definition
 * seat block: count +0x2E4, block +0x2E8, stride 284, name at +4), it scans all units for the one whose
 * parent (+0x11C) is the vehicle and whose seat index (+0x2F0) is that seat, then checks whether that unit is
 * a member of the object list. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/unit_datum.h"
#include "headers/vehicle_definition.h"
#include "headers/unit_seat.h"
#include "headers/global_tag_instances.h"
#include "headers/object_iterator.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

extern int stricmp(const char *a, const char *b);
extern int object_list_get_first(int object_list_index, int *iterator);
extern int object_list_get_next(int object_list_index, int *reference_index);
extern void object_iterator_new(object_iterator *iterator, uint32_t type_flags, uint8_t flags);

uint8_t unit_scripting_vehicle_test_seat_list(int unit_index, const char *seat_name, int object_list_index)
{
    int result = 0;
    if ( unit_index == -1 )
        return 0;

    object_datum *unit = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    vehicle_definition *definition = TAG_GET(vehicle_definition, unit->definition_index);
    int seat_count = definition->unit.seats.count;
    if ( seat_count <= 0 )
        return 0;

    unit_seat *seats = (unit_seat *)definition->unit.seats.address;
    for ( int seat = 0; seat < seat_count; seat = (int16_t)(seat + 1) )
    {
        const char *seat_definition_name = seats[seat].label;
        if ( stricmp(seat_name, seat_definition_name) )
            continue;

        object_iterator iterator;
        object_iterator_new(&iterator, object_mask_unit, 0);
        unsigned char occupant_found = 0;
        for ( unit_datum *occupant = object_iterator_next(&iterator); occupant;
              occupant = object_iterator_next(&iterator) )
        {
            if ( occupant->object.parent_object_index == unit_index && occupant->unit.parent_seat_index == seat )
            {
                occupant_found = 1;
                break;
            }
        }
        if ( !occupant_found )
            continue;

        int list_iterator[4];
        for ( int member = object_list_get_first(object_list_index, list_iterator);
              member != -1;
              member = object_list_get_next(object_list_index, list_iterator) )
        {
            if ( iterator.index == member )
            {
                result = 1;
                break;
            }
        }
    }
    return result;
}
