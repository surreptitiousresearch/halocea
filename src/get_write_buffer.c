#include <stdint.h>
#include "headers/simple_decompressor_definition.h"

void * get_write_buffer(simple_decompressor_definition *self, int16_t write_buffer_index)
{
    return self->write_buffers[write_buffer_index];
}
