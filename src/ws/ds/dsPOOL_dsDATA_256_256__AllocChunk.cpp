#include "dsPOOL.h"
#include "dsDATA.h"
#include "ds_assert_boundary.h"

// boundary — debug allocator: dlMalloc(size, callsite_file, callsite_line).
extern "C" void *dlMalloc(unsigned int size, const char *file, unsigned int line);

// dsPOOL<dsDATA,256,256>::AllocChunk @ 0x82614460
// Allocate a fresh chunk of 256 dsDATA values and thread every slot onto the free list. While
// free, each value's leading `storage` word aliases the "next free index" link.
template<>
void dsPOOL<dsDATA, 256, 256>::AllocChunk()
{
    if (!IGNORE_STRONG_ASSERT && this->numChunks >= 256)
        STRONG_ASSERT_DUMMY().Crash(
            "numChunks < MAX_CHUNKS",
            "D:\\Projects\\code\\common\\INCL.SYS\\ds/ds_pool.h",
            200,
            empty_string);

    dsDATA *chunk = (dsDATA *)dlMalloc(0x800u, this->cl.file, this->cl.line); // 256 * 8
    this->chunkTable[this->numChunks].data = chunk;
    int baseIndex = this->numChunks << 8; // numChunks * 256

    for (int i = 0; i < 255; ++i)
        chunk[i].storage = baseIndex + 1 + i;
    chunk[255].storage = this->emptyChain;

    this->emptyChain = baseIndex;
    this->numChunks = this->numChunks + 1;
}
