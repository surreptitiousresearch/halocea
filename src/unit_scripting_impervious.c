/* unit_scripting_impervious @0x83800A98 — set or clear the "impervious" flag (unit flags +0x204 bit 0x800000)
 * on every unit in an object list. */

#include <stdint.h>
#include "headers/unit_datum.h"
#include "headers/unit_flags.h"
#include "headers/object_type.h"

extern int object_list_get_first(int object_list_index, int *iterator);
extern int object_list_get_next(int object_list_index, int *reference_index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);

void unit_scripting_impervious(int object_list_index, uint8_t impervious)
{
    int iterator[2];
    for ( int object_index = object_list_get_first(object_list_index, iterator);
          object_index != -1;
          object_index = object_list_get_next(object_list_index, iterator) )
    {
        unit_datum *unit = object_try_and_get_and_verify_type(object_index, object_mask_unit);
        if ( unit )
        {
            unsigned int flags = unit->unit.flags | (1u << _unit_impervious_bit);
            if ( !impervious )
                flags = unit->unit.flags & ~(1u << _unit_impervious_bit);
            unit->unit.flags = flags;
        }
    }
}
