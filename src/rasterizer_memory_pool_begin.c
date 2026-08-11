#include "headers/blam_data_globals.h"
/* rasterizer_memory_pool_begin @0x83791DA0 — resets the per-frame rasterizer scratch-memory
 * allocation cursor. */


void rasterizer_memory_pool_begin(void)
{
    global_memory_index = 0;
}
