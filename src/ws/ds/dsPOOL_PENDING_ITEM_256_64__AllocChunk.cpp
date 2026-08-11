#include "dsPOOL.h"
#include "dsEVENT_MGR.h"
#include "ds_assert_boundary.h"

// boundary — debug allocator: dlMalloc(size, callsite_file, callsite_line).
extern "C" void *dlMalloc(unsigned int size, const char *file, unsigned int line);

// dsPOOL<dsEVENT_MGR::PENDING_ITEM,256,64>::AllocChunk @ 0x82613B90
// Allocate a fresh chunk of 256 PENDING_ITEMs and thread every slot onto the free list. While
// free, each item's leading `idxEvent` word aliases the "next free index" link.
template<>
void dsPOOL<dsEVENT_MGR::PENDING_ITEM, 256, 64>::AllocChunk()
{
    if (!IGNORE_STRONG_ASSERT && this->numChunks >= 64)
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
            "numChunks < MAX_CHUNKS",
            "D:\\Projects\\code\\common\\INCL.SYS\\ds/ds_pool.h",
            200,
            empty_string);

    dsEVENT_MGR::PENDING_ITEM *chunk =
        (dsEVENT_MGR::PENDING_ITEM *)dlMalloc(0xC00u, this->cl.file, this->cl.line); // 256 * 12
    this->chunkTable[this->numChunks].data = chunk;
    int baseIndex = this->numChunks << 8; // numChunks * 256

    for (int i = 0; i < 255; ++i)
        chunk[i].idxEvent = baseIndex + 1 + i;
    chunk[255].idxEvent = this->emptyChain;

    this->emptyChain = baseIndex;
    this->numChunks = this->numChunks + 1;
}
