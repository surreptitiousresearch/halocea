/* get_read_request_size @0x838083A8 — retail stub: unconditionally returns 0x20000. */

#include "headers/cache_copy_read_request.h"
#include "headers/simple_decompressor_definition.h"
#include "headers/cache_copy_buffer_sizes.h"

int get_read_request_size(simple_decompressor_definition *self, cache_copy_read_request *request)
{
    return FILE_BLOCK_SIZE;
}
