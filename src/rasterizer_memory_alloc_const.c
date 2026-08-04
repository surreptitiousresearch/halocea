/* rasterizer_memory_alloc_const 0x83791E28 — tail-call thunk to rasterizer_memory_alloc; identical behaviour,
 * only the return type is const-qualified for callers that copy immutable data into the rasterizer arena. */

extern void *rasterizer_memory_alloc(const void *src, unsigned int size);

const void *rasterizer_memory_alloc_const(const void *src, unsigned int size)
{
    return rasterizer_memory_alloc(src, size);
}
