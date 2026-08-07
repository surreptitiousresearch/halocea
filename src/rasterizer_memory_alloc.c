#include <string.h>
#include "headers/blam_data_globals.h"
#include "headers/rasterizer_limits.h"
/* rasterizer_memory_alloc 0x83791DB0 — bump-allocate `size` bytes from the rasterizer's fixed 0x18000-byte scratch
 * pool, optionally copying `src` into the allocation. Returns null when the pool would overflow. */

/* global_memory_pool is a POINTER variable (canonical def in rasterizer_memory_pool_initialize.c);
 * disasm 0x83791DEC loads it (lwz), it is not an array base. Previous extern-array decl was wrong. */
extern void *global_memory_pool;

void *rasterizer_memory_alloc(const void *src, unsigned int size)
{
    void *result = nullptr;
    unsigned int offset = global_memory_index;
    if ( global_memory_index + size <= RASTERIZER_MEMORY_POOL_SIZE )
    {
        global_memory_index += size;
        result = (unsigned char *)global_memory_pool + offset;
        if ( src )
            memcpy(result, src, size);
    }
    return result;
}
