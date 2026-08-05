#include "dsPOOL.h"
#include "dsEVENT_MGR.h"
#include "ds_assert_boundary.h"

// dsPOOL<dsEVENT_MGR::ITEM_REC,256,256>::Get @ 0x82613278
// Non-const accessor: resolve a slot index to a reference to its stored ITEM_REC. The slot lives in
// chunk (idx / 256) at position (idx % 256). Asserts idx >= 0, that its chunk is allocated, and
// that the in-chunk position is within CHUNK_SIZE before dereferencing.
template<>
dsEVENT_MGR::ITEM_REC &dsPOOL<dsEVENT_MGR::ITEM_REC, 256, 256>::Get(int idx)
{
    if (!IGNORE_STRONG_ASSERT && idx < 0)
        STRONG_ASSERT_DUMMY().Crash(
            "idx >= 0",
            "D:\\Projects\\code\\common\\INCL.SYS\\ds/ds_pool.h",
            175,
            empty_string);
    int idxChunk = idx / 256;
    int idxElem = idx % 256;
    if (!IGNORE_STRONG_ASSERT) {
        if (idxChunk >= this->numChunks)
            STRONG_ASSERT_DUMMY().Crash(
                "idxChunk < numChunks",
                "D:\\Projects\\code\\common\\INCL.SYS\\ds/ds_pool.h",
                180,
                empty_string);
        else if (idxElem >= 256)
            STRONG_ASSERT_DUMMY().Crash(
                "idxElem < CHUNK_SIZE",
                "D:\\Projects\\code\\common\\INCL.SYS\\ds/ds_pool.h",
                181,
                empty_string);
    }
    return this->chunkTable[idxChunk].data[idxElem];
}
