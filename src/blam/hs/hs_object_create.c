/* hs_object_create @0x837F7CE0 — instantiate the scenario object with the given name index if it is not
 * already present in the world. */

#include <stdint.h>

extern int object_index_from_name_index(int16_t name_index);
extern void object_new_by_name(int16_t name_index);

void hs_object_create(int16_t object_name_index)
{
    if ( object_name_index != -1 && object_index_from_name_index(object_name_index) == -1 )
        object_new_by_name(object_name_index);
}
