#include <stdint.h>
#include <string.h>

void static_array_new(uint8_t *count, void *elements, int16_t element_size, int16_t maximum_count)
{
    *count = 0;
    memset(elements, -1, element_size * maximum_count);
}
