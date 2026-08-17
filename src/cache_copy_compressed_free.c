/* cache_copy_compressed_free @0x838080A0 */
#include <stdint.h>
#include "headers/simple_decompressor_definition.h"

/* Takes void* to match zlib's free_func (z_stream.zfree); stored back as the byte-pointer
 * next-allocation cursor. */
void cache_copy_compressed_free(void *opaque, void *address)
{
    global_self->next_allocation = address;
}
