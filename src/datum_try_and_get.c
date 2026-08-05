/* datum_try_and_get @ 0x836FAF80 — return a pointer to the datum at `index` in a data_array, or
 * null if the index is invalid: the sentinel -1, a negative (high-bit-set) handle, an out-of-range
 * slot, an empty slot (salt word 0), or a salt mismatch against the handle's high word. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/datum_index.h"

/* attested: returns a generic datum pointer (callers type it per-array); disasm returns the raw
 * slot address in r3. `data` is never written through — const. */
void *datum_try_and_get(const data_array *data, int index)
{
    if ( index != -1 && (int16_t)index >= 0 && (int16_t)index < data->maximum_count )
    {
        int16_t *datum = (int16_t *)((char *)data->data + data->size * (int16_t)index);
        if ( *datum )
        {
            /* identifier salt = handle's high 16 bits (value semantics) */
        if ( !DATUM_INDEX_TO_IDENTIFIER(index) || *datum == (int16_t)DATUM_INDEX_TO_IDENTIFIER(index) )
                return datum;
        }
    }
    return 0;
}
