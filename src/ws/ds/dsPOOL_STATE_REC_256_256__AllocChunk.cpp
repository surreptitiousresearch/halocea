#include "dsPOOL.h"
#include "dsSTATE_MGR.h"
#include "ds_assert_boundary.h"

// boundary — debug allocator: dlMalloc(size, callsite_file, callsite_line).
extern "C" void *dlMalloc(unsigned int size, const char *file, unsigned int line);

// dsPOOL<dsSTATE_MGR::STATE_REC,256,256>::AllocChunk @ 0x82614388
// Allocate a fresh chunk of 256 STATE_RECs and thread every slot onto the free list. While free,
// each record's leading `idxData` word aliases the "next free index" link.
template<>
void dsPOOL<dsSTATE_MGR::STATE_REC, 256, 256>::AllocChunk()
{
    if (!IGNORE_STRONG_ASSERT && this->numChunks >= 256)
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
            "numChunks < MAX_CHUNKS",
            "D:\\Projects\\code\\common\\INCL.SYS\\ds/ds_pool.h",
            200,
            empty_string);

    dsSTATE_MGR::STATE_REC *chunk =
        (dsSTATE_MGR::STATE_REC *)dlMalloc(0x2800u, this->cl.file, this->cl.line); // 256 * 40
    this->chunkTable[this->numChunks].data = chunk;
    int baseIndex = this->numChunks << 8; // numChunks * 256

    for (int i = 0; i < 255; ++i)
        chunk[i].idxData = baseIndex + 1 + i;
    chunk[255].idxData = this->emptyChain;

    this->emptyChain = baseIndex;
    this->numChunks = this->numChunks + 1;
}
