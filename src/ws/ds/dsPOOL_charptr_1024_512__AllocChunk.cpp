#include "dsPOOL.h"
#include "ds_assert_boundary.h"

// boundary — debug allocator: dlMalloc(size, callsite_file, callsite_line).
extern "C" void *dlMalloc(unsigned int size, const char *file, unsigned int line);

// dsPOOL<char const*,1024,512>::AllocChunk @ 0x82528820
// Allocate a fresh chunk of 1024 elements and thread every slot onto the free list. Each free
// slot's element word aliases the "next free index" link; the last slot chains to the previous
// free-list head, and the new chunk's base index becomes the new head.
template<>
void dsPOOL<const char *, 1024, 512>::AllocChunk()
{
    if (!IGNORE_STRONG_ASSERT && this->numChunks >= 512)
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
            "numChunks < MAX_CHUNKS",
            "D:\\Projects\\code\\common\\incl.sys\\ds\\ds_pool.h",
            200,
            empty_string);

    const char **chunk = (const char **)dlMalloc(0x1000u, this->cl.file, this->cl.line); // 1024 * 4
    this->chunkTable[this->numChunks].data = chunk;
    int baseIndex = this->numChunks << 10; // numChunks * 1024

    // Link slots 0..1022 to the next slot index; slot 1023 to the old free-list head.
    for (int i = 0; i < 1023; ++i)
        chunk[i] = (const char *)(baseIndex + 1 + i);
    chunk[1023] = (const char *)this->emptyChain;

    this->emptyChain = baseIndex;
    this->numChunks = this->numChunks + 1;
}
