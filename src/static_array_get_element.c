#include <stdint.h>
/* static_array_get_element @0x83784B28 — indexes a flat element array (confirmed via decompiler:
 * `&elements[element_size * index]`). `count` is unused. */

void * static_array_get_element(uint8_t count, const void *elements, int16_t element_size, int16_t index)
{
    return (char *)elements + element_size * index;
}
