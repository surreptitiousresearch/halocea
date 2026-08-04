#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

struct apDEFRAG_CHUNK_DESC; // boundary — ws-engine ap: defrag pool chunk descriptor (element is a pointer)

// dsVECTOR<apDEFRAG_CHUNK_DESC*,8>::operator[] (non-const) @ 0x827B3468
// Bounds-checked mutable access; returns a reference to the idx-th element pointer.
template<>
apDEFRAG_CHUNK_DESC *&dsVECTOR<apDEFRAG_CHUNK_DESC *, 8>::operator[](int idx)
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
