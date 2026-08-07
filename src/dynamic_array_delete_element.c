/* dynamic_array_delete_element @0x83784988 — remove the element at `index`, shifting the tail down to keep
 * the array contiguous, then shrink the backing allocation to the new count. */

#include <string.h>
#include "headers/dynamic_array.h"

extern void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line);

void dynamic_array_delete_element(dynamic_array *array, int index)
{
    int new_count = array->count - 1;
    array->count = new_count;
    if ( index < new_count )
        memmove(
            (char *)array->elements + array->element_size * index,
            (char *)array->elements + array->element_size * index + array->element_size,
            (new_count - index) * array->element_size);
    array->elements = dlRealloc(
        array->elements, array->element_size * array->count,
        "D:\\Projects\\code\\HCEX\\sources\\memory\\array.c", 0x9Cu);
}
