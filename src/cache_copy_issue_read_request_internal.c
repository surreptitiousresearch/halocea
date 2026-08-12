#include <stdint.h>
#include "headers/simple_decompressor_definition.h"
#include "headers/cache_copy_buffer_sizes.h"

extern void XPhysicalProtect(void *addr, int size, unsigned int flags);
extern void cache_copy_issue_read_internal(simple_decompressor_definition *self, void *buffer, unsigned int size, int offset, int16_t read_buffer_index);

void cache_copy_issue_read_request_internal(simple_decompressor_definition *self, cache_copy_read_request *request, int16_t read_buffer_index)
{
    int read_bytes_left = self->read_bytes_left;
    /* DEVIATION: decompiler derives buffer index by pointer arithmetic; equivalent to read_buffer_index */
    void *buf = self->read_buffers[(short)(((char *)request - (char *)self - 2684) >> 1)];
    if (read_bytes_left >= FILE_BLOCK_SIZE)
        read_bytes_left = FILE_BLOCK_SIZE;
    request->read_sequence_index = self->current_read_sequence_index;
    XPhysicalProtect(buf, read_bytes_left, 4);
    cache_copy_issue_read_internal(self, buf, read_bytes_left, self->current_read_offset, read_buffer_index);
    short seq = self->current_read_sequence_index;
    self->current_read_offset += read_bytes_left;
    self->current_read_sequence_index = seq + 1;
    self->read_bytes_left -= read_bytes_left;
}
