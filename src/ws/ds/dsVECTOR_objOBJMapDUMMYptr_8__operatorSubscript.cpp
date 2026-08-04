#include "dsVECTOR.h"
#include "dsObjOBJVecMapDUMMY.h"
#include "ds_assert_boundary.h"

// dsVECTOR<...objOBJ-map DUMMY*,8>::operator[] (non-const) @ 0x826852D8
// Bounds-checked mutable access; returns a reference to the idx-th element pointer.
template<>
dsObjOBJVecMapDUMMY *&dsVECTOR<dsObjOBJVecMapDUMMY *, 8>::operator[](int idx)
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
