/* dynamic_array_resize @0x837847B0 — grow or shrink a dynamic_array to exactly `count` elements via
 * dlRealloc (annotated with the original array.c source location), zero-filling any newly added tail.
 * Fails on a negative count or when the reallocation outcome is inconsistent with the request (NULL for a
 * nonzero count, or non-NULL for zero — the compiled (count != 0) != (block != NULL) test). */

#include <stdint.h>
#include "headers/dynamic_array.h"
#include <string.h>

extern void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line);

uint8_t dynamic_array_resize(dynamic_array *array, int count)
{
    if ( count < 0 )
        return 0;
    if ( count == array->count )
        return 1;

    char *elements = dlRealloc(array->elements, array->element_size * count,
                                       "D:\\Projects\\code\\HCEX\\sources\\memory\\array.c", 0x2Cu);
    if ( (count != 0) != (elements != 0) )
        return 0;

    if ( count > array->count )
        memset(&elements[array->element_size * array->count], 0,
               (count - array->count) * array->element_size);

    array->count = count;
    array->elements = elements;
    return 1;
}
