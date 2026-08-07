/* dynamic_array_add_element @0x837848E0 — grows a dynamic_array by one element (reallocating its backing
 * storage), zero-initializes the new element, and returns its index (or -1 on overflow/allocation failure).
 */

#include <string.h>
#include "headers/dynamic_array.h"

extern void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line);

int dynamic_array_add_element(dynamic_array *array)
{
    int count = array->count;
    int new_index = -1;

    if ( count != 0x7FFFFFFF )
    {
        char *new_elements = dlRealloc(array->elements, array->element_size * (count + 1),
            "D:\\Projects\\code\\HCEX\\sources\\memory\\array.c", 0x67u);

        if ( new_elements )
        {
            new_index = array->count;
            memset(&new_elements[array->element_size * new_index], 0, array->element_size);
            array->count = count + 1;
            array->elements = new_elements;
        }
    }

    return new_index;
}
