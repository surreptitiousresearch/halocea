#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

struct strmVID_BUFFER; // boundary — strm subsystem video buffer (element is a pointer)

// dsVECTOR<strmVID_BUFFER*,8>::operator[] (const) @ 0x82698C00
// Bounds-checked read-only access; returns a reference to the idx-th element pointer.
template<>
strmVID_BUFFER *const &dsVECTOR<strmVID_BUFFER *, 8>::operator[](int idx) const
{
    if (!IGNORE_STRONG_ASSERT && (idx < 0 || idx >= this->nElem))
        STRONG_ASSERT2_HELPER::asserd<int, int>(
            "idx >= 0 && idx < nElem",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            557,
            "idx", idx,
            "nElem", this->nElem);
    return this->pData[idx];
}
