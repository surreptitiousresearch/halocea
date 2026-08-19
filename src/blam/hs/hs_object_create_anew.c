/* hs_object_create_anew @0x837F8990 — re-create a scenario object: if it already exists (and is not a
 * player) delete it, then instantiate it fresh. */

#include <stdint.h>

extern int object_index_from_name_index(int16_t name_index);
extern uint8_t object_is_or_contains_player(int object_index);
extern void object_delete(int object_index);
extern void object_new_by_name(int16_t name_index);

void hs_object_create_anew(int16_t object_name_index)
{
    if ( object_name_index == -1 )
        return;
    int existing = object_index_from_name_index(object_name_index);
    if ( existing != -1 && !object_is_or_contains_player(existing) )
        object_delete(existing);
    if ( object_index_from_name_index(object_name_index) == -1 )
        object_new_by_name(object_name_index);
}
