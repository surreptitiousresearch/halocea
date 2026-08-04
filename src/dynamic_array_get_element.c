/* dynamic_array_get_element @0x83784958 — indexes a dynamic_array's backing storage using the array's own
 * element_size (the `element_size` parameter is unused; the array's own field is authoritative per disasm). */

#include "headers/dynamic_array.h"

void * dynamic_array_get_element(const dynamic_array *array, int index, int element_size)
{
    return (char *)array->elements + array->element_size * index;
}
