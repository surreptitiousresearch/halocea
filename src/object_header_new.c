/* object_header_new @0x836ECD98 — allocate a new object header datum and its live object block.
 *
 * Allocates a datum slot in the object-header data_array (at a specific index if requested, otherwise
 * the next free one), then allocates a `size`-byte block from the object memory pool for the live
 * object_datum, records the block size, and zero-fills it. On pool-allocation failure the datum slot
 * is released and -1 is returned. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/memory_pool.h"
#include "headers/blam_data_globals.h"
#include <string.h>

extern int datum_new(data_array *data);
extern int datum_new_at_index(data_array *data, int index);
extern void datum_delete(data_array *data, int index);
extern uint8_t memory_pool_block_allocate(memory_pool *pool, void **reference, int size);

int object_header_new(data_array *data, int index, int16_t size)
{
    int datum_index;
    if ( index == -1 )
        datum_index = datum_new(data);
    else
        datum_index = datum_new_at_index(data, index);

    if ( datum_index == -1 )
        return datum_index;

    object_header_datum *header = DATA_ARRAY_ELEMENT(data, object_header_datum, datum_index);
    if ( memory_pool_block_allocate(object_memory_pool, (void **)&header->datum, size) )
    {
        header->data_size = size;
        memset(header->datum, 0, size);
        return datum_index;
    }

    datum_delete(data, datum_index);
    return -1;
}
