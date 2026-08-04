#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

struct apDEFRAG_POOL; // boundary — ws-engine ap: defragmentable memory pool (element is a pointer)

// dsVECTOR<apDEFRAG_POOL*,8>::ShrinkResize @ 0x823F1D68
// Shrink the live count to `size`. The element is a raw pointer (trivially destructible), so
// the generic per-element destructor loop body is empty; only the count is reset.
template<>
void dsVECTOR<apDEFRAG_POOL *, 8>::ShrinkResize(int size)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            270,
            "size",
            size);
    if (size < this->nElem) {
        for (int i = size; i < this->nElem; ++i)
            ; // trivially destructible pointer element; destructor loop emitted as a no-op
        this->nElem = size;
    }
}
