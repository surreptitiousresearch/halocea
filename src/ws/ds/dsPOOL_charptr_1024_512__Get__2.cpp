#include "dsPOOL.h"
#include "ds_assert_boundary.h"

// dsPOOL<char const*,1024,512>::Get @ 0x82529300
// Const accessor: resolve a slot index to a reference to its stored element. The slot lives in
// chunk (idx / 1024) at position (idx % 1024) of that chunk's element array. Asserts the index is
// in range (non-negative and within an allocated chunk) before dereferencing.
template<>
const char *const &dsPOOL<const char *, 1024, 512>::Get(int idx) const
{
    if (!IGNORE_STRONG_ASSERT && (idx < 0 || idx / 1024 >= this->numChunks))
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
            "IsIndexValid(idx)",
            "D:\\Projects\\code\\common\\incl.sys\\ds\\ds_pool.h",
            189,
            empty_string);
    return this->chunkTable[idx / 1024].data[idx % 1024];
}
