#include "headers/simple_decompressor_definition.h"

void * get_read_request_buffer(simple_decompressor_definition *self, cache_copy_read_request *request)
{
    return self->read_buffers[(__int16)(((char *)request - (char *)self - 2684) >> 1)];
}
