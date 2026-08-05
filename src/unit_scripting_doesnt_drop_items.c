/* unit_scripting_doesnt_drop_items @0x836C9E08 — flag every unit in an object list so it does not drop its
 * inventory items on death (unit flags +0x204 bit 0x100000). */

#include "headers/unit_datum.h"
#include "headers/unit_flags.h"
#include "headers/object_type.h"

extern int object_list_get_first(int object_list_index, int *reference_index);
extern int object_list_get_next(int object_list_index, int *reference_index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);

void unit_scripting_doesnt_drop_items(int object_list_index)
{
    int iterator[4];
    for ( int object_index = object_list_get_first(object_list_index, iterator);
          object_index != -1;
          object_index = object_list_get_next(object_list_index, iterator) )
    {
        unit_datum *unit = object_try_and_get_and_verify_type(object_index, object_mask_unit);
        if ( unit )
            unit->unit.flags |= (1u << _unit_doesnt_drop_items_bit);
    }
}
