/* object_header_block_allocate @0x836ECEB8 — grow an object's variable-length data block by `size`
 * bytes and record the new sub-block at `block_reference_offset` within the object data. Reallocates
 * the object's memory-pool block to the new total, writes a {size, prior_total} reference pair at the
 * given offset, zeroes the appended region, and returns 1; returns 0 if the reallocation fails.
 *
 * Object header datum: running data size in ->data_size, payload pointer in ->datum. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/blam_data_globals.h"
#include "headers/memory_pool.h"
extern void *memset(void *dst, int value, unsigned int n);

extern uint8_t memory_pool_block_reallocate(memory_pool *pool, void **reference, int new_size);

uint8_t object_header_block_allocate(int object_index, int16_t block_reference_offset, int16_t size)
{
    object_header_datum *header = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index);
    __int16 prior_total;
    object_header_block_reference *reference;

    if ( !memory_pool_block_reallocate(object_memory_pool, &header->datum,
                header->data_size + size) )
        return 0;

    prior_total = header->data_size;
    header->data_size = (__int16)(size + prior_total);

    reference = (object_header_block_reference *)((char *)header->datum + block_reference_offset);
    reference->offset = prior_total;
    reference->size = size;

    memset((char *)header->datum + prior_total, 0, size);
    return 1;
}
