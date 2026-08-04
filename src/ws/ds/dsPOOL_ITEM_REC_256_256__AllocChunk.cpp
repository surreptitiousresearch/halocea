#include "dsPOOL.h"
#include "dsEVENT_MGR.h"
#include "ds_assert_boundary.h"

// boundary — debug allocator: dlMalloc(size, callsite_file, callsite_line).
extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);

// dsPOOL<dsEVENT_MGR::ITEM_REC,256,256>::AllocChunk @ 0x82613F38
// Allocate a fresh chunk of 256 ITEM_RECs and thread every slot onto the free list. While free,
// each record's leading `name.id` word aliases the "next free index" link.
template<>
void dsPOOL<dsEVENT_MGR::ITEM_REC, 256, 256>::AllocChunk()
{
    if (!IGNORE_STRONG_ASSERT && this->numChunks >= 256)
        STRONG_ASSERT_DUMMY().Crash(
            "numChunks < MAX_CHUNKS",
            "D:\\Projects\\code\\common\\INCL.SYS\\ds/ds_pool.h",
            200,
            dsStrongAssertMessage);

    dsEVENT_MGR::ITEM_REC *chunk =
        (dsEVENT_MGR::ITEM_REC *)dlMalloc(0x2400u, this->cl.file, this->cl.line); // 256 * 36
    this->chunkTable[this->numChunks].data = chunk;
    int baseIndex = this->numChunks << 8; // numChunks * 256

    for (int i = 0; i < 255; ++i)
        chunk[i].name.id = (const char *)(baseIndex + 1 + i);
    chunk[255].name.id = (const char *)this->emptyChain;

    this->emptyChain = baseIndex;
    this->numChunks = this->numChunks + 1;
}
