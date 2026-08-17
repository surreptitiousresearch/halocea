/* cache_copy_issue_read @0x83808A68 */
#include <stdint.h>
#include "headers/simple_decompressor_definition.h"

#include "headers/cache_copy_read_request.h"
extern void cache_copy_issue_read_request_internal(simple_decompressor_definition *self, cache_copy_read_request *request, int16_t read_buffer_index);

void cache_copy_issue_read(simple_decompressor_definition *self, int16_t read_buffer_index)
{
    cache_copy_issue_read_request_internal(self, &self->read_requests[read_buffer_index], read_buffer_index);
}
