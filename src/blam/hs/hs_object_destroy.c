/* hs_object_destroy @0x837F7D28 — delete an object, unless it is (or contains) a player. */

#include <stdint.h>

extern uint8_t object_is_or_contains_player(int object_index);
extern void object_delete(int object_index);

void hs_object_destroy(int object_index)
{
    if ( object_index != -1 && !object_is_or_contains_player(object_index) )
        object_delete(object_index);
}
