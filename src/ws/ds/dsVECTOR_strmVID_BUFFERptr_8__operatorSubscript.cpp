#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

struct strmVID_BUFFER; // boundary — strm subsystem video buffer (element is a pointer)

// dsVECTOR<strmVID_BUFFER*,8>::operator[] (non-const) @ 0x82698B68
// Bounds-checked mutable access; returns a reference to the idx-th element pointer.
template<>
strmVID_BUFFER *&dsVECTOR<strmVID_BUFFER *, 8>::operator[](int idx)
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
