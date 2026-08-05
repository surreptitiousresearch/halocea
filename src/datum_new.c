/* datum_new @ 0x836FB3D8 — allocate a datum slot in a data_array. Scans from the cached first-free
 * index for an empty slot (salt word 0), clears it, stamps the array's rolling identifier (wrapping
 * -1 -> 0x8000 so the high bit stays clear), and updates the free index and counts. Returns the
 * packed handle (identifier in the high word, absolute index in the low word) or -1 if full. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/datum_index.h"

extern void *memset(void *dst, int value, unsigned int n);

int datum_new(data_array *data)
{
    int size = data->size;
    int16_t maximum_count = data->maximum_count;
    int first_free_absolute_index = data->first_free_absolute_index;
    int16_t *slot = (int16_t *)((char *)data->data + first_free_absolute_index * size);

    if ( first_free_absolute_index < maximum_count )
    {
        int16_t next_identifier;
        int16_t actual_count;
        int count;

        while ( *slot )
        {
            slot = (int16_t *)((char *)slot + size);
            first_free_absolute_index = (int16_t)(first_free_absolute_index + 1);
            if ( first_free_absolute_index >= data->maximum_count )
                return -1;
        }

        memset(slot, 0, size);
        *slot = data->next_identifier;
        next_identifier = data->next_identifier;
        data->next_identifier = next_identifier + 1;
        if ( next_identifier == -1 )
            data->next_identifier = 0x8000;

        actual_count = data->actual_count;
        count = data->count;
        data->first_free_absolute_index = first_free_absolute_index + 1;
        data->actual_count = actual_count + 1;
        if ( count <= first_free_absolute_index )
            data->count = first_free_absolute_index + 1;

        /* pack identifier salt into high word, absolute index into low word */
        return BUILD_DATUM_INDEX(*slot, first_free_absolute_index);
    }
    return -1;
}
