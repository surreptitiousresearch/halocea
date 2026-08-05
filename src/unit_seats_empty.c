/* unit_seats_empty @0x836CEA78 — return 1 if every seat of a unit (vehicle) is unoccupied, 0 if any seat has
 * a rider. Reads the seat count from the unit's tag definition; for each seat index it scans all units
 * looking for one whose object.parent_object_index is this unit and whose unit.parent_seat_index matches. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/object_iterator.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

extern void object_iterator_new(object_iterator *iterator, uint32_t type_flags, uint8_t flags);
extern void *object_iterator_next(object_iterator *iterator);

uint8_t unit_seats_empty(int unit_index)
{
    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    unit_definition *definition = TAG_GET(unit_definition, unit->definition_index);

    int seat_count = definition->unit.seats.count;
    if ( seat_count <= 0 )
        return 1;

    for ( int16_t seat_index = 0; seat_index < seat_count; seat_index = (int16_t)(seat_index + 1) )
    {
        char occupied = 0;
        object_iterator iterator;
        object_iterator_new(&iterator, object_mask_unit, 0);
        unit_datum *rider = object_iterator_next(&iterator);
        if ( rider )
        {
            while ( rider->object.parent_object_index != unit_index || rider->unit.parent_seat_index != seat_index )
            {
                rider = object_iterator_next(&iterator);
                if ( !rider )
                    goto next_seat;
            }
            occupied = 1;
        }
    next_seat:
        if ( occupied == 1 )
            return 0;
    }
    return 1;
}
