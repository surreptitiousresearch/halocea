#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

// boundary — ds::MAP<objOBJ*,dsVECTOR<objOBJ*,8>,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::DUMMY,
// a hash-map node record keying object pointers to per-key object vectors. Opaque here;
// the element is a pointer. Distinct tag avoids ODR collision with other MAP DUMMY records.
struct dsObjOBJVecMapDUMMY;

// dsVECTOR<...objOBJ-map DUMMY*,8>::Back @ 0x82685DA0
// Reference to the last live element pointer (index nElem-1); assert non-empty first.
template<>
dsObjOBJVecMapDUMMY *&dsVECTOR<dsObjOBJVecMapDUMMY *, 8>::Back()
{
    if (!IGNORE_STRONG_ASSERT && this->nElem <= 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "nElem > 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            527,
            "nElem",
            this->nElem);
    return this->pData[this->nElem - 1];
}
