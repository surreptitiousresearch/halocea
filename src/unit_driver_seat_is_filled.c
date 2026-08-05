/* unit_driver_seat_is_filled @0x836CAC78 — true if any "driver" seat (seat flag bit 0x4) of a unit is
 * occupied. Walks the unit tag's seat array; for each driver seat it iterates all unit objects looking
 * for one parented to this unit at that seat index. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/object_iterator.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/unit_seat.h"
#include "headers/unit_seat_flags.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

extern void object_iterator_new(object_iterator *iterator, uint32_t type_flags, uint8_t flags);
extern void *object_iterator_next(object_iterator *iterator);

uint8_t unit_driver_seat_is_filled(int unit_index)
{
    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    unit_definition *definition = TAG_GET(unit_definition, unit->definition_index);

    int seat_count = definition->unit.seats.count;
    if ( seat_count <= 0 )
        return 0;

    int16_t seat_index = 0;
    while ( 1 )
    {
        if ( (((unit_seat *)definition->unit.seats.address)[seat_index].flags & (1u << _unit_seat_is_driver_bit)) != 0 )
        {
            char occupied = 0;
            object_iterator iterator;
            object_iterator_new(&iterator, object_mask_unit, 0);
            unit_datum *child = object_iterator_next(&iterator);
            if ( child )
            {
                while ( child->object.parent_object_index != unit_index
                     || child->unit.parent_seat_index != seat_index )
                {
                    child = object_iterator_next(&iterator);
                    if ( !child )
                        goto next_seat;
                }
                occupied = 1;
            }
        next_seat:
            if ( occupied == 1 )
                return 1;
        }
        seat_index = (int16_t)(seat_index + 1);
        if ( seat_index >= definition->unit.seats.count )
            return 0;
    }
}
