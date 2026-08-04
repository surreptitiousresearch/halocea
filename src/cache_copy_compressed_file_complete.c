#include <stdint.h>
#include "headers/simple_decompressor_definition.h"

extern unsigned int WaitForSingleObject(void *handle, unsigned int milliseconds);

uint8_t cache_copy_compressed_file_complete(void)
{
    return WaitForSingleObject(global_self->copy_complete_event, 0) == 0;
}
