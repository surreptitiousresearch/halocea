/* static_array_delete_element @0x83784B40 — remove element `index` from a fixed-capacity packed array:
 * shift the tail down over it, decrement count, and stamp the freed trailing slot with 0xFF bytes. */

#include <stdint.h>
#include <string.h>

void static_array_delete_element(uint8_t *count, void *elements, int16_t element_size, int16_t index)
{
    unsigned __int8 new_count = *count - 1;
    char *bytes = elements;

    *count = new_count;
    if ( index < new_count )
    {
        memmove(&bytes[element_size * index], &bytes[element_size * index + element_size],
            (new_count - index) * element_size);
    }
    memset(&bytes[*count * element_size], -1, element_size);
}
