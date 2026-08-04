/* object_name_list_lookup @0x836EE158 — resolve a scenario object-name index to an object index.
 *
 * Returns the object currently bound to the given name slot, or -1 if the name index is out of range. */

#include <stdint.h>
#include "headers/blam_data_globals.h"
#include "headers/scenario_constants.h"

int object_name_list_lookup(int16_t name_index)
{
    if ( (unsigned int)name_index >= MAXIMUM_OBJECT_NAMES_PER_SCENARIO )
        return -1;
    return object_name_list[name_index];
}
