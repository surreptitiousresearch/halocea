/* object_set_object_index_for_name_index @0x836EC6E0 — bind the scenario object-name slot to a live object
 * index, so script references by name resolve to the running object. Bounds-checked against the scenario's
 * object_names tag block. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/blam_data_globals.h"


void object_set_object_index_for_name_index(int16_t name_index, int object_index)
{
    if ( name_index >= 0 && name_index < global_scenario->object_names.count )
        object_name_list[name_index] = object_index;
}
