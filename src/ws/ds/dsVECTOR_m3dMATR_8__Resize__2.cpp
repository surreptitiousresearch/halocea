#include "dsVECTOR.h"
#include "../m3d/m3dMATR.h"
#include "ds_assert_boundary.h"

// dsVECTOR<m3dMATR,8>::Resize(int) @ 0x82861F18
template<>
void dsVECTOR<m3dMATR, 8>::Resize(int size)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            211,
            "size", size);

    int nElem = this->nElem;
    if (size >= nElem) {
        if (size > nElem && size > this->allocated)
            this->Realloc(size);
    }
    this->nElem = size;
}
