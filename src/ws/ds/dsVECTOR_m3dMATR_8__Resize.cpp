#include <string.h>
#include "dsVECTOR.h"
#include "../m3d/m3dMATR.h"
#include "ds_assert_boundary.h"

// dsVECTOR<m3dMATR,8>::Resize(int,const m3dMATR&) @ 0x82861C78
// Resize the live count to `size`, fill-copying `val` (a POD memcpy) into any newly exposed slots;
// shrinking has no per-element teardown.
template<>
void dsVECTOR<m3dMATR, 8>::Resize(int size, const m3dMATR &val)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            238,
            "size", size);

    int nElem = this->nElem;
    if (size < nElem) {
        this->nElem = size;
        return;
    }
    if (size == nElem)
        return;
    if (size > this->allocated)
        this->Realloc(size);

    int grownFrom = this->nElem;
    for (int index = grownFrom; index < size; ++index)
        this->pData[index] = val;
    this->nElem = size;
}
