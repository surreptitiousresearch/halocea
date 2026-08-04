#include "dsPOOL.h"
#include "ds_assert_boundary.h"

// dsPOOL<char const*,1024,512>::Get @ 0x825283B8
// Non-const accessor: resolve a slot index to a reference to its stored element. The slot lives in
// chunk (idx / 1024) at position (idx % 1024) of that chunk's element array. Asserts idx >= 0, that
// its chunk is allocated, and that the in-chunk position is within CHUNK_SIZE before dereferencing.
template<>
const char *&dsPOOL<const char *, 1024, 512>::Get(int idx)
{
    if (!IGNORE_STRONG_ASSERT && idx < 0)
        STRONG_ASSERT_DUMMY().Crash(
            "idx >= 0",
            "D:\\Projects\\code\\common\\incl.sys\\ds\\ds_pool.h",
            175,
            dsStrongAssertMessage);
    int idxChunk = idx / 1024;
    int idxElem = idx % 1024;
    if (!IGNORE_STRONG_ASSERT) {
        if (idxChunk >= this->numChunks)
            STRONG_ASSERT_DUMMY().Crash(
                "idxChunk < numChunks",
                "D:\\Projects\\code\\common\\incl.sys\\ds\\ds_pool.h",
                180,
                dsStrongAssertMessage);
        else if (idxElem >= 1024)
            STRONG_ASSERT_DUMMY().Crash(
                "idxElem < CHUNK_SIZE",
                "D:\\Projects\\code\\common\\incl.sys\\ds\\ds_pool.h",
                181,
                dsStrongAssertMessage);
    }
    return this->chunkTable[idxChunk].data[idxElem];
}
