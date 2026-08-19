/* cache_copy_buffer_size @0x83807DF0 */
#include <stdint.h>
#include "headers/cache_copy_buffer_sizes.h"
#include "headers/simple_decompressor_definition.h"

/* Win32 import (kernel32 boundary; system <windows.h> collides with repo Win32 shim types). */
extern int SetThreadPriority(void *hThread, int nPriority);

int cache_copy_buffer_size(uint8_t should_block)
{
    simple_decompressor_definition *self = global_self;

    self->blocking = should_block;
    SetThreadPriority(self->copy_thread, should_block != 0);
    return TOTAL_BUFFER_SIZE;
}
