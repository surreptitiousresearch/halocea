#include <stdint.h>
#include "headers/simple_decompressor_definition.h"

#include "headers/cache_copy_read_request.h"
extern void cache_copy_issue_read_request_internal(simple_decompressor_definition *self, cache_copy_read_request *request, int16_t read_buffer_index);

void cache_copy_initialize_read_buffers(simple_decompressor_definition *self)
{
    __int16 buffer_index = 0;

    for ( int i = 0; i < 8; i++ )
    {
        cache_copy_issue_read_request_internal(self, &self->read_requests[i], buffer_index);
        buffer_index = (__int16)(i + 1);
        self->overlapped_in_use_flags[0] |= 1 << (i & 0x1F);
    }
}
