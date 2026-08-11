#include "dsPOOL.h"
#include "dsEVENT_MGR.h"
#include "ds_assert_boundary.h"

// dsPOOL<dsEVENT_MGR::ITEM_HANDLER,512,256>::Get @ 0x826133E0
// Non-const accessor: resolve a slot index to a reference to its stored ITEM_HANDLER. The slot
// lives in chunk (idx / 512) at position (idx % 512). Asserts idx >= 0, that its chunk is
// allocated, and that the in-chunk position is within CHUNK_SIZE before dereferencing.
template<>
dsEVENT_MGR::ITEM_HANDLER &dsPOOL<dsEVENT_MGR::ITEM_HANDLER, 512, 256>::Get(int idx)
{
    if (!IGNORE_STRONG_ASSERT && idx < 0)
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
            "idx >= 0",
            "D:\\Projects\\code\\common\\INCL.SYS\\ds/ds_pool.h",
            175,
            empty_string);
    int idxChunk = idx / 512;
    int idxElem = idx % 512;
    if (!IGNORE_STRONG_ASSERT) {
        if (idxChunk >= this->numChunks)
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
                "idxChunk < numChunks",
                "D:\\Projects\\code\\common\\INCL.SYS\\ds/ds_pool.h",
                180,
                empty_string);
        else if (idxElem >= 512)
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
                "idxElem < CHUNK_SIZE",
                "D:\\Projects\\code\\common\\INCL.SYS\\ds/ds_pool.h",
                181,
                empty_string);
    }
    return this->chunkTable[idxChunk].data[idxElem];
}
