#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

struct instCONSTRUCTOR_NODE_RELATIONS; // boundary — ws-engine prop: instance-constructor node relation record (element is a pointer)

// dsVECTOR<instCONSTRUCTOR_NODE_RELATIONS*,8>::operator[] (non-const) @ 0x826DFD60
// Bounds-checked mutable access; returns a reference to the idx-th element pointer.
template<>
instCONSTRUCTOR_NODE_RELATIONS *&dsVECTOR<instCONSTRUCTOR_NODE_RELATIONS *, 8>::operator[](int idx)
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
