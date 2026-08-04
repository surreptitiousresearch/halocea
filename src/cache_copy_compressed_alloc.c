#include <stdint.h>
#include "headers/simple_decompressor_definition.h"
#include "headers/blam_data_globals.h"


/* Returns void* to match zlib's alloc_func (z_stream.zalloc); the allocation is carved as a
 * byte pointer internally and implicitly widens to void* on return. */
void * cache_copy_compressed_alloc(void *opaque, unsigned int items, unsigned int size)
{
    unsigned char *result = global_self->next_allocation;
    global_self->next_allocation = result + items * size;
    return result;
}
