#include <stdint.h>
/* hs_object_list_get_element @0x837F8290 — return the object at the given position in a HaloScript object
 * list, or -1 if the index is past the end. */

extern int object_list_get_first(int object_list_index, int *reference_index);
extern int object_list_get_next(int object_list_index, int *reference_index);

int hs_object_list_get_element(int object_list_index, int16_t element_index)
{
    int iterator[2];
    int object_index = object_list_get_first(object_list_index, iterator);
    int remaining = element_index;
    while ( remaining > 0 )
    {
        if ( object_index == -1 )
            break;
        object_index = object_list_get_next(object_list_index, iterator);
        remaining = (int16_t)(remaining - 1);
    }
    return object_index;
}
