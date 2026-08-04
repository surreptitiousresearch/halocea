/* rasterizer_memory_pool_initialize @ 0x83791D48
   Allocates the 96 KiB rasterizer scratch memory pool via the Blam allocator
   (dlMalloc), tagged with the original source location. */

#include <stdint.h>

extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);

void *global_memory_pool;

/* Caller re-normalizes r3 with `clrlwi r11,r3,24` => 8-bit (Blam boolean) return. */
uint8_t rasterizer_memory_pool_initialize(void)
{
    global_memory_pool = dlMalloc(0x18000u,
        "D:\\Projects\\code\\HCEX\\sources\\rasterizer\\rasterizer_memory_pool.c", 0x13);
    return global_memory_pool != 0;
}
