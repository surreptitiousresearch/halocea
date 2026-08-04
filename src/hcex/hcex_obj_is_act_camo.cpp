/* hcex_obj_is_act_camo @0x83681E30 — is a Blam object (or its parent) currently in active camouflage? Reads
 * the object datum via the object-header data-array. If the object has a parent that is a unit (type in
 * {biped, vehicle} — mask (1<<type)&3) whose unit flags have the active-camo bit 0x10 set, the object counts
 * as camouflaged. Otherwise the object itself is tested the same way. The unit-flags location and camo bit
 * 0x10 match hcex_plr_is_act_camo. */

#include "../headers/data_array.h"
#include "../headers/object_header_datum.h"
#include "../headers/unit_datum.h"
#include "../headers/unit_flags.h"
#include "../headers/object_type.h"

extern data_array *object_header_data;

extern "C" int hcex_obj_is_act_camo(int id)
{
    unit_datum *object = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, id)->datum;

    int parent_index = object->object.parent_object_index;
    if ( parent_index != -1 )
    {
        unit_datum *parent = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, parent_index)->datum;
        if ( ((1 << parent->object.type) & object_mask_unit) != 0
            && (parent->unit.flags & (1u << _unit_active_camouflaged_bit)) != 0 )
            return 1;
    }

    if ( ((1 << object->object.type) & object_mask_unit) == 0 )
        return 0;
    return (object->unit.flags & (1u << _unit_active_camouflaged_bit)) != 0;
}
