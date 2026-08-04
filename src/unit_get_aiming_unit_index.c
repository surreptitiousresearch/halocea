/* unit_get_aiming_unit_index @0x836C9838 — if the unit is seated in a parent vehicle and its seat definition has
 * the delegating seat flags (_unit_seat_is_invisible_bit or _unit_seat_is_gunner_bit), aiming is delegated to the parent unit; return the parent's
 * index. Otherwise the unit aims for itself. Parent at object.parent_object_index, seat index at unit.parent_seat_index; the parent's
 * vehicle tag holds the seat block (unit_definition->unit.seats, unit_seat stride 284 per seat). */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/unit_datum.h"
#include "headers/object_datum.h"
#include "headers/unit_definition.h"
#include "headers/unit_seat.h"
#include "headers/unit_seat_flags.h"
#include "headers/blam_data_globals.h"


int unit_get_aiming_unit_index(int unit_index)
{
    if ( unit_index != -1 )
    {
        unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
        int parent_index = unit->object.parent_object_index;
        if ( parent_index != -1 )
        {
            short seat_index = unit->unit.parent_seat_index;
            if ( seat_index != -1 )
            {
                object_datum *parent = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, parent_index)->datum;
                unit_definition *parent_def = TAG_GET(unit_definition, parent->definition_index);
                unsigned int seat_flags = ((const unit_seat *)parent_def->unit.seats.address)[seat_index].flags;
                if ( ((seat_flags >> _unit_seat_is_invisible_bit) & 1) != 0
                     || ((seat_flags >> _unit_seat_is_gunner_bit) & 1) != 0 )
                    return parent_index;
            }
        }
    }
    return unit_index;
}
