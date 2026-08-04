#include <stdint.h>
#include <string.h>

int16_t static_array_add_element(uint8_t *count, void *elements, int16_t element_size, int16_t maximum_count)
{
    unsigned __int8 index = *count;

    if ( index >= maximum_count )
        return -1;

    *count = index + 1;
    memset((char *)elements + index * element_size, 0, element_size);
    return index;
}
