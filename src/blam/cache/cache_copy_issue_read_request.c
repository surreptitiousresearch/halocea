/* cache_copy_issue_read_request @0x83808A50 */
#include <stdint.h>
#include "headers/simple_decompressor_definition.h"
#include "headers/cache_copy_read_request.h"

extern void cache_copy_issue_read_request_internal(simple_decompressor_definition *self, cache_copy_read_request *request, int16_t read_buffer_index);

void cache_copy_issue_read_request(simple_decompressor_definition *self,
                                    cache_copy_read_request *request)
{
    cache_copy_issue_read_request_internal(self, request,
        ((char *)request - (char *)self - 2684) >> 1);
}
