#include "dsPOOL.h"
#include "dsSTATE_MGR.h"
#include "ds_assert_boundary.h"

// dsPOOL<dsSTATE_MGR::STATE_REC,256,256>::Get @ 0x82613810
// Non-const accessor: resolve a slot index to a reference to its stored STATE_REC. The slot lives
// in chunk (idx / 256) at position (idx % 256). Asserts idx >= 0, that its chunk is allocated, and
// that the in-chunk position is within CHUNK_SIZE before dereferencing.
template<>
dsSTATE_MGR::STATE_REC &dsPOOL<dsSTATE_MGR::STATE_REC, 256, 256>::Get(int idx)
{
    if (!IGNORE_STRONG_ASSERT && idx < 0)
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
            "idx >= 0",
            "D:\\Projects\\code\\common\\INCL.SYS\\ds/ds_pool.h",
            175,
            empty_string);
    int idxChunk = idx / 256;
    int idxElem = idx % 256;
    if (!IGNORE_STRONG_ASSERT) {
        if (idxChunk >= this->numChunks)
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
                "idxChunk < numChunks",
                "D:\\Projects\\code\\common\\INCL.SYS\\ds/ds_pool.h",
                180,
                empty_string);
        else if (idxElem >= 256)
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
                "idxElem < CHUNK_SIZE",
                "D:\\Projects\\code\\common\\INCL.SYS\\ds/ds_pool.h",
                181,
                empty_string);
    }
    return this->chunkTable[idxChunk].data[idxElem];
}
