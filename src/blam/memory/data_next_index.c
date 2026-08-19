/* data_next_index @ 0x836FAE78 — iterate occupied slots of a data_array. Given the previous absolute
 * index (or -1 to start), scan forward for the next non-empty slot (salt word != 0) and return its
 * packed handle (salt in the high word, absolute index in the low word), or -1 when none remain. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/datum_index.h"

int data_next_index(const data_array *data, int16_t index)
{
    int absolute_index = (int16_t)(index + 1);
    int count;
    int size;
    const int16_t *slot;

    if ( absolute_index < 0 )
        return -1;

    count = data->count;
    if ( absolute_index >= count )
        return -1;

    size = data->size;
    slot = (const int16_t *)((const char *)data->data + (int16_t)size * absolute_index);
    while ( !*slot )
    {
        slot = (const int16_t *)((const char *)slot + size);
        absolute_index = (int16_t)(absolute_index + 1);
        if ( absolute_index >= count )
            return -1;
    }
    return BUILD_DATUM_INDEX(*slot, absolute_index);
}
