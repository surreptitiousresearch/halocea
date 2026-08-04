/* dynamic_array_new @0x83784798 — initializes a dynamic_array to empty with the given element size. */

#include "headers/dynamic_array.h"

void dynamic_array_new(dynamic_array *array, int element_size)
{
    array->element_size = element_size;
    array->count = 0;
    array->elements = 0;
}
