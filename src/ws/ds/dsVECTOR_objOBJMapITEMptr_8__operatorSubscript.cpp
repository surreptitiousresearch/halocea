#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

struct dsObjOBJVecMapITEM; // boundary — ds::MAP<objOBJ*,dsVECTOR<objOBJ*,8>,...>::ITEM (opaque; pointer element)

// dsVECTOR<...objOBJ-map ITEM*,8>::operator[] (non-const) @ 0x826851A8
// Bounds-checked mutable access; returns a reference to the idx-th element pointer.
template<>
dsObjOBJVecMapITEM *&dsVECTOR<dsObjOBJVecMapITEM *, 8>::operator[](int idx)
{
    if (!IGNORE_STRONG_ASSERT && (idx < 0 || idx >= this->nElem))
        STRONG_ASSERT2_HELPER::asserd<int, int>(
            "idx >= 0 && idx < nElem",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            567,
            "idx", idx,
            "nElem", this->nElem);
    return this->pData[idx];
}
