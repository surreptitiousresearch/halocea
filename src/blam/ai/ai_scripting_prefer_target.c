/* ai_scripting_prefer_target @0x837711D8 — set or clear the "preferred AI target" flag (object flags bit
 * 0x800, at object +0x204) on every unit in the object list, biasing actors toward attacking them. */

#include <stdint.h>
#include "headers/unit_datum.h"
#include "headers/object_type.h"
#include "headers/unit_flags.h"

extern int object_list_get_first(int object_list_index, int *reference_index);
extern int object_list_get_next(int object_list_index, int *reference_index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);

void ai_scripting_prefer_target(int object_list_index, uint8_t prefer)
{
    int iterator[2];
    for ( int object_index = object_list_get_first(object_list_index, iterator);
          object_index != -1;
          object_index = object_list_get_next(object_list_index, iterator) )
    {
        unit_datum *object = object_try_and_get_and_verify_type(object_index, object_mask_unit);
        if ( object )
        {
            unsigned int flags = object->unit.flags;           /* object +0x204 */
            object->unit.flags = prefer
                ? (flags | (1u << _unit_preferred_target_bit))
                : (flags & ~(1u << _unit_preferred_target_bit));
        }
    }
}
