#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

struct instCONSTRUCTOR_NODE; // boundary — ws-engine prop: instance-constructor graph node (element is a pointer)

// dsVECTOR<instCONSTRUCTOR_NODE*,8>::operator[] (non-const) @ 0x826DFB98
// Bounds-checked mutable access; returns a reference to the idx-th element pointer.
template<>
instCONSTRUCTOR_NODE *&dsVECTOR<instCONSTRUCTOR_NODE *, 8>::operator[](int idx)
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
