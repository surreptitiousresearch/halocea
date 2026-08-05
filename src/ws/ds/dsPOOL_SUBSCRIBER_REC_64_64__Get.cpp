#include "dsPOOL.h"
#include "dsEVENT_MGR.h"
#include "ds_assert_boundary.h"

// dsPOOL<dsEVENT_MGR::SUBSCRIBER_REC,64,64>::Get @ 0x826136A8
// Non-const accessor: resolve a slot index to a reference to its stored SUBSCRIBER_REC. The slot
// lives in chunk (idx / 64) at position (idx % 64). Asserts idx >= 0, that its chunk is allocated,
// and that the in-chunk position is within CHUNK_SIZE before dereferencing.
template<>
dsEVENT_MGR::SUBSCRIBER_REC &dsPOOL<dsEVENT_MGR::SUBSCRIBER_REC, 64, 64>::Get(int idx)
{
    if (!IGNORE_STRONG_ASSERT && idx < 0)
        STRONG_ASSERT_DUMMY().Crash(
            "idx >= 0",
            "D:\\Projects\\code\\common\\INCL.SYS\\ds/ds_pool.h",
            175,
            empty_string);
    int idxChunk = idx / 64;
    int idxElem = idx % 64;
    if (!IGNORE_STRONG_ASSERT) {
        if (idxChunk >= this->numChunks)
            STRONG_ASSERT_DUMMY().Crash(
                "idxChunk < numChunks",
                "D:\\Projects\\code\\common\\INCL.SYS\\ds/ds_pool.h",
                180,
                empty_string);
        else if (idxElem >= 64)
            STRONG_ASSERT_DUMMY().Crash(
                "idxElem < CHUNK_SIZE",
                "D:\\Projects\\code\\common\\INCL.SYS\\ds/ds_pool.h",
                181,
                empty_string);
    }
    return this->chunkTable[idxChunk].data[idxElem];
}
