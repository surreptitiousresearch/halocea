/* unit_get_seat_label @0x836CF478 — return the label string for the seat a unit occupies. If the unit has
 * no parent or no seat assigned, return the base seat label indexed by the unit's animation base seat
 * type. Otherwise look up the parent object's tag, index its unit.seats block by the seat index, and
 * return the seat definition's label. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/unit_seat.h"
#include "headers/blam_data_globals.h"


const char *unit_get_seat_label(int object_index)
{
    unit_datum *object_data = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum);

    int parent_index = object_data->object.parent_object_index;
    if ( parent_index == -1 )
        return base_seat_labels[object_data->unit.animation.base_seat_index];

    int16_t seat_index = object_data->unit.parent_seat_index;
    if ( seat_index == -1 )
        return base_seat_labels[object_data->unit.animation.base_seat_index];

    unit_datum *parent_data = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, parent_index)->datum);
    /* parent tag's unit.seats block indexed by seat */
    unit_definition *parent_definition = TAG_GET(unit_definition, parent_data->definition_index);
    return ((const unit_seat *)parent_definition->unit.seats.address)[seat_index].label;
}
