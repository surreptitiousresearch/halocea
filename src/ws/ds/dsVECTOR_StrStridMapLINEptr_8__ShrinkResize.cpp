#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

// boundary — dsLF_SORTED_MAP<const char*,dsSTRID,dsSTR_HASH,dsSTR_CMP>::LINE hash-chain node.
// Opaque here; the vector element is a pointer to it.
struct dsStrStridMapLINE;

// dsVECTOR<dsStrStridMapLINE*,8>::ShrinkResize @ 0x8252A198
// Shrink the live count to `size`. The element is a raw pointer (trivially destructible), so
// the generic per-element destructor loop body is empty; only the count is reset.
template<>
void dsVECTOR<dsStrStridMapLINE *, 8>::ShrinkResize(int size)
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
