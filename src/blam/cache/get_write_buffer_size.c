/* get_write_buffer_size @0x838083E8 — retail stub: unconditionally returns 0x400000. */

#include <stdint.h>
#include "headers/simple_decompressor_definition.h"
#include "headers/cache_copy_buffer_sizes.h"

int get_write_buffer_size(simple_decompressor_definition *self, int16_t write_buffer_index)
{
    return WRITE_FILE_BLOCK_SIZE;
}
