/* ai_scripting_migrate_by_unit @0x837709E0 — migrate the actors controlling each unit in an object list
 * (and each unit's biped/vehicle child objects) into the target encounter, then refresh team status. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_datum.h"
#include "headers/object_header_datum.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

extern int object_list_get_first(int object_list_index, int *reference_index);
extern int object_list_get_next(int object_list_index, int *reference_index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void ai_scripting_migrate_by_unit_internal(int unit_index, int ai_index, uint8_t generate_stimulus, uint8_t advancing_stimulus);
extern void ai_update_team_status(void);
extern void encounters_update_dirty_status(void);

void ai_scripting_migrate_by_unit(int object_list_index, int ai_index)
{
    if ( object_list_index == -1 || ai_index == -1 )
        return;

    int iterator[16];
    for ( int object_index = object_list_get_first(object_list_index, iterator);
          object_index != -1;
          object_index = object_list_get_next(object_list_index, iterator) )
    {
        object_datum *object = object_try_and_get_and_verify_type(object_index, object_mask_unit);
        if ( !object )
            continue;

        ai_scripting_migrate_by_unit_internal(object_index, ai_index, 0, 0);

        /* Also migrate the unit's biped/vehicle child objects (type in object_mask_unit). */
        for ( int child = object->object.first_child_object_index; child != -1; )
        {
            object_datum *child_object =
                DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, child)->datum;
            if ( ((1 << child_object->object.type) & object_mask_unit) != 0 )
                ai_scripting_migrate_by_unit_internal(child, ai_index, 0, 0);
            child = child_object->object.next_object_index;    /* next child */
        }
    }
    ai_update_team_status();
    encounters_update_dirty_status();
}
