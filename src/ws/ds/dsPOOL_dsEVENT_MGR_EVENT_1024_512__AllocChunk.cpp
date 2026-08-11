#include "dsPOOL.h"
#include "dsEVENT_MGR.h"
#include "ds_assert_boundary.h"

extern "C" void *dlMalloc(unsigned int size, const char *file, unsigned int line); // boundary — allocator

// dsPOOL<dsEVENT_MGR::EVENT,1024,512>::AllocChunk @ 0x826140E0
// Allocate a fresh chunk of 1024 EVENT records (76 bytes each => 0x13000) and thread every slot
// onto the free list. While free, each record's leading `id` word aliases the "next free index"
// link.
template<>
void dsPOOL<dsEVENT_MGR::EVENT, 1024, 512>::AllocChunk()
{
    if (!IGNORE_STRONG_ASSERT && this->numChunks >= 512)
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
            "numChunks < MAX_CHUNKS",
            "D:\\Projects\\code\\common\\INCL.SYS\\ds/ds_pool.h",
            200,
            empty_string);

    dsEVENT_MGR::EVENT *chunk =
        (dsEVENT_MGR::EVENT *)dlMalloc(0x13000u, this->cl.file, this->cl.line); // 1024 * 76
    this->chunkTable[this->numChunks].data = chunk;
    int baseIndex = this->numChunks << 10; // numChunks * 1024

    for (int i = 0; i < 1023; ++i)
        chunk[i].id = baseIndex + 1 + i;
    chunk[1023].id = this->emptyChain;

    this->emptyChain = baseIndex;
    this->numChunks = this->numChunks + 1;
}
