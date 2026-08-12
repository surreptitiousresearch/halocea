/* cache_copy_issue_write @0x83808AC0 — issue an async write of up to 4MB from a write buffer, unprotecting
 * the target buffer first, then advance the write cursor and remaining-bytes counter.
 *
 * DEVIATION: the decompiler indexes the write buffer via raw byte arithmetic on `src_name`
 * (`src_name[4*(write_buffer_index+610)]`); 610*4 = 2440 is exactly the DB's own offset for `write_buffers`,
 * so this is `write_buffers[write_buffer_index]` (the array's declared [1] bound is just documentation —
 * indexed beyond it here). */

#include <stdint.h>
#include "headers/simple_decompressor_definition.h"
#include "headers/cache_copy_buffer_sizes.h"

extern void XPhysicalProtect(void *lpAddress, unsigned int dwSize, unsigned int flNewProtect);
extern void cache_copy_issue_write_internal(simple_decompressor_definition *self, void *buffer, int size, int offset, int16_t write_buffer_index);

void cache_copy_issue_write(simple_decompressor_definition *self, int16_t write_buffer_index)
{
    int write_bytes_left = self->write_bytes_left;
    void *buffer = self->write_buffers[write_buffer_index];

    if ( write_bytes_left >= WRITE_FILE_BLOCK_SIZE )
        write_bytes_left = WRITE_FILE_BLOCK_SIZE;

    XPhysicalProtect(buffer, WRITE_FILE_BLOCK_SIZE, 2u);
    cache_copy_issue_write_internal(self, buffer, write_bytes_left, self->current_write_offset, write_buffer_index);

    self->write_bytes_left -= write_bytes_left;
    self->current_write_offset += write_bytes_left;
}
