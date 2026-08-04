/* object_type_update_baseline @0x83702578 — invoke the most-derived part definition's update_baseline
 * callback for an object (used to refresh its network baseline state). Scans part_definitions from the
 * deepest sub-type (index 15) up to the super-type and calls the first callback found.
 *
 * Object type selects the object_type_definition (object datum +180 = object.type). */

#include "headers/object_type_definition.h"
#include "headers/object_type.h"
#include "headers/object_datum.h"
#include "headers/blam_data_globals.h"

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);

void object_type_update_baseline(int object_index)
{
    object_datum *object = object_try_and_get_and_verify_type(object_index, object_mask_all);
    int i;

    if ( !object )
        return;

    for ( i = 15; i >= 0; --i )
    {
        object_type_definition *part = object_type_definitions[object->object.type]->part_definitions[i];
        if ( part && part->update_baseline )
        {
            part->update_baseline(object_index);
            return;
        }
    }
}
