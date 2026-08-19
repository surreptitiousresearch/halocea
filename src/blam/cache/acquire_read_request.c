/* acquire_read_request @0x838082E8 — find the read_requests[] slot (of 8) matching `read_sequence_index`
 * whose I/O has completed (overlapped_completed_flags bit for that slot set), make its read buffer
 * writable again (XPhysicalProtect), and return it. Null if no such slot exists.
 *
 * Note: `overlapped_completed_flags` is a plain 32-bit bitmask (simple_decompressor_definition.h); the
 * decompiled `self->overlapped_completed_flags[v4 >> 5]` array indexing is only ever evaluated at index 0
 * since the slot loop bound is 8 (v4>>5 is always 0 for v4 in [0,8)), so it's read here as a plain bit test. */

#include <stdint.h>
#include "headers/simple_decompressor_definition.h"
#include "headers/cache_copy_buffer_sizes.h"

extern int XPhysicalProtect(void *address, unsigned int size, unsigned int protect);

cache_copy_read_request * acquire_read_request(simple_decompressor_definition *self, int16_t read_sequence_index)
{
    for ( int i = 0; i < 8; ++i )
    {
        cache_copy_read_request *request = &self->read_requests[i];

        if ( request->read_sequence_index == read_sequence_index
          && ((1u << (i & 0x1F)) & self->overlapped_completed_flags[0]) != 0 )
        {
            XPhysicalProtect(self->read_buffers[i], FILE_BLOCK_SIZE, 2u);
            return request;
        }
    }

    return 0;
}
