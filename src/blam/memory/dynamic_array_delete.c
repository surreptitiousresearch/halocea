/* dynamic_array_delete @0x83784878 */
#include "headers/dynamic_array.h"

extern void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line);

void dynamic_array_delete(dynamic_array *array)
{
    void *elements = array->elements;

    array->element_size = -1;
    array->count = -1;
    if (elements)
        array->elements = dlRealloc(elements, 0, "D:\\Projects\\code\\HCEX\\sources\\memory\\array.c", 0x50u);
}
