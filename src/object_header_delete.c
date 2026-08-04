/* object_header_delete @0x836ECE48 — release an object header slot: free its memory-pool block (the
 * object data at header+8), delete the datum from the object header array, and clear the header's
 * block pointer and flags byte.
 *
 * Object header datum: pointer-to-object-data at +8. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/blam_data_globals.h"

#include "headers/memory_pool.h"
extern void memory_pool_block_free(memory_pool *pool, void **reference);
extern void datum_delete(data_array *data, int index);

void object_header_delete(data_array *data, int object_index)
{
    /* recovered: raw 4*(u16)i + 4*((2i)&0x1FFFE) is 12*(u16)i, the object_header_datum stride */
    object_header_datum *header = DATA_ARRAY_ELEMENT(data, object_header_datum, object_index);

    if ( header->datum )
        memory_pool_block_free(object_memory_pool, &header->datum);
    datum_delete(data, object_index);
    header->datum = nullptr;
    header->flags = 0;
}
