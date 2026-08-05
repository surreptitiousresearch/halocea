#include <stdint.h>
#include "headers/bit_vector.h"
#include "headers/simple_decompressor_definition.h"

extern void cache_copy_issue_read_request_internal(simple_decompressor_definition *self, cache_copy_read_request *request, int16_t read_buffer_index);

void release_read_request(simple_decompressor_definition *self, cache_copy_read_request *request)
{
    int16_t slot_index = (int16_t)(((char *)request - (char *)self - 2684) >> 1);

    /* overlapped_completed_flags is one 32-bit word; slot_index/32 is always 0 for the 8-slot read_requests
     * array, but indexed here to mirror the disasm's word-array addressing. */
    BIT_VECTOR_CLEAR_FLAG(self->overlapped_completed_flags, slot_index);
    request->read_sequence_index = -1;
    cache_copy_issue_read_request_internal(self, request, slot_index);
}
