/* unit_scripting_unit_riders @0x836CA0D8 — build a HaloScript object list of every unit riding the given
 * unit. Walks the child chain (first child at unit +0x118, sibling at +0x114) and adds each child that is a
 * biped or vehicle (object types 0/1) currently occupying a seat (seat index +0x2F0 != 0xFFFF). Returns the
 * new object list, or -1 for an invalid unit or list-allocation failure. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

extern int object_list_new(void);
extern void object_list_add(int object_list_index, int object_index);

int unit_scripting_unit_riders(int unit_index)
{
    if ( unit_index == -1 )
        return -1;

    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    int object_list = object_list_new();
    if ( object_list == -1 )
        return -1;

    for ( int child = unit->object.first_child_object_index; child != -1; )
    {
        unit_datum *child_data = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, child)->datum;
        if ( ((1 << child_data->object.type) & object_mask_unit) != 0
             && (unsigned __int16)child_data->unit.parent_seat_index != 0xFFFF )
            object_list_add(object_list, child);
        child = child_data->object.next_object_index;
    }
    return object_list;
}
