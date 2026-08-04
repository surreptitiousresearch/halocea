#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

struct strmVID_BUFFER; // boundary — strm subsystem video buffer (element is a pointer)

// dsVECTOR<strmVID_BUFFER*,8>::Front @ 0x82698A20
// Reference to the first live element pointer; assert the vector is non-empty first.
template<>
strmVID_BUFFER *&dsVECTOR<strmVID_BUFFER *, 8>::Front()
{
    if (!IGNORE_STRONG_ASSERT && this->nElem <= 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "nElem > 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            497,
            "nElem",
            this->nElem);
    return this->pData[0];
}
