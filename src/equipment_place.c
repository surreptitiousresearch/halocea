/* equipment_place @0x837B5DF4 — applies the scenario placement's misc_flags to a newly-placed equipment
 * object's runtime flags: scenario flag _equipment_created_at_rest_bit mirrors into object.flags as
 * _object_at_rest_bit and (unconditionally) the cannot-be-garbage|shadowless pair, and scenario flag
 * _equipment_does_accelerate_bit clears item.flags _item_does_not_accelerate_bit; when not created at rest,
 * nudges the object's z position up slightly so it doesn't clip the floor.
 *
 * Fields resolved against equipment_datum (object.flags @+16, item.flags @+500, object.position @+92).
 * Scenario misc_flags bits DB-verified via equipment_flags ($E7D11F01...); item.flags bit via item_flags
 * ($982827FB...). */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/equipment_datum.h"
#include "headers/scenario_equipment_datum.h"
#include "headers/object_flags.h"
#include "headers/equipment_flags.h"
#include "headers/item_flags.h"
#include "headers/blam_data_globals.h"


void equipment_place(int equipment_index, scenario_equipment_datum *scenario_equipment)
{
    equipment_datum *object =
        (equipment_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, equipment_index)->datum;

    unsigned int flags = object->object.flags;
    flags |= (1u << _object_at_rest_bit);
    if ( (scenario_equipment->object.misc_flags & (1u << _equipment_created_at_rest_bit)) == 0 )
        flags &= ~(1u << _object_at_rest_bit);
    object->object.flags = flags;

    unsigned int item_flags = object->item.flags;
    /* Deviation preserved: the binary re-writes object.flags here (not item.flags), OR-ing in the
       cannot-be-garbage|shadowless pair, partly overwriting the masked value stored just above. */
    object->object.flags = flags | ((1u << _object_cannot_be_garbage_bit) | (1u << _object_shadowless_bit));

    item_flags |= (1u << _item_does_not_accelerate_bit);
    if ( (scenario_equipment->object.misc_flags & (1u << _equipment_does_accelerate_bit)) != 0 )
        item_flags &= ~(1u << _item_does_not_accelerate_bit);
    object->item.flags = item_flags;

    if ( (scenario_equipment->object.misc_flags & (1u << _equipment_created_at_rest_bit)) == 0 )
        object->object.position.n[2] += 0.05f;
}
