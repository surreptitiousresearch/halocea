#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

// boundary — ds::MAP<objOBJ*,dsVECTOR<objOBJ*,8>,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::DUMMY,
// a hash-map node record keying object pointers to per-key object vectors. Opaque here; the
// element is a pointer. Distinct tag matches dsVECTOR_objOBJMapDUMMYptr_8__Back (no ODR collision).
struct dsObjOBJVecMapDUMMY;

// dsVECTOR<...objOBJ-map DUMMY*,8>::ShrinkResize @ 0x82685B30
// Shrink the live count to `size`. The element is a raw pointer (trivially destructible), so
// the generic per-element destructor loop body is empty; only the count is reset.
template<>
void dsVECTOR<dsObjOBJVecMapDUMMY *, 8>::ShrinkResize(int size)
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
