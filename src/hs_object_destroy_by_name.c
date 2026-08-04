/* hs_object_destroy_by_name @0x837F7D88 — script hook: delete the object bound to a scenario object
 * name, unless it is (or carries) a player. No-op for an unset name or missing object. */

#include <stdint.h>

extern int object_index_from_name_index(int16_t name_index);
extern uint8_t object_is_or_contains_player(int object_index);
extern void object_delete(int object_index);

void hs_object_destroy_by_name(int16_t object_name_index)
{
    if ( object_name_index != -1 )
    {
        int object_index = object_index_from_name_index(object_name_index);
        if ( object_index != -1 && !object_is_or_contains_player(object_index) )
            object_delete(object_index);
    }
}
