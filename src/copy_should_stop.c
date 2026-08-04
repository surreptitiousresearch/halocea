#include <stdint.h>
#include "headers/simple_decompressor_definition.h"

extern unsigned int WaitForSingleObject(void *handle, unsigned int milliseconds);

uint8_t copy_should_stop(void)
{
    return WaitForSingleObject(global_self->copy_stop_event, 0) == 0;
}
