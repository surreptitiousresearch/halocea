#include <stdint.h>
#include "headers/blam_data_globals.h"
#include "headers/scenario_constants.h"
/* object_index_from_name_index @0x836EF898 — look up the object currently bound to a scenario name index, or
 * -1 if the index is out of range or unbound. */


int object_index_from_name_index(int16_t name_index)
{
    /* disasm 836EF89C/836EF8A4: two signed compares (blt 0 / bge 0x200), not one unsigned compare */
    if ( name_index < 0 || name_index >= MAXIMUM_OBJECT_NAMES_PER_SCENARIO )
        return -1;
    return object_name_list[name_index];
}
