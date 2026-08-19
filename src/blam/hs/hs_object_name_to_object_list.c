/* hs_object_name_to_object_list @0x8368D628 — HaloScript coercion: resolve a scenario object-name index
 * (high 16 bits of the argument) to a live object, then wrap it in a one-element object list. Returns -1
 * when the name maps to no current object. */

#include <stdint.h>

extern int object_index_from_name_index(int16_t name_index);
extern int object_list_new(void);
extern void object_list_add(int object_list_index, int object_index);

int hs_object_name_to_object_list(int object_name_index)
{
    int object_list = -1;
    int object_index = object_index_from_name_index((short)((unsigned int)object_name_index >> 16));

    if (object_index != -1)
    {
        object_list = object_list_new();
        object_list_add(object_list, object_index);
    }
    return object_list;
}
