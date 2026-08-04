#include "dsVECTOR.h"
#include "dsDATA.h"
#include "dsDATA_TYPE.h"
#include "ds_assert_boundary.h"

// dsVECTOR<dsDATA,8>::ShrinkResize @ 0x825A85B0
// Shrink the live count to `size`, destroying every element from `size` up to the old count. Each
// dsDATA carries its own runtime type descriptor; destroying it dispatches that descriptor's
// Destroy virtual on the element's storage word (skipping empty elements whose type is null).
template<>
void dsVECTOR<dsDATA, 8>::ShrinkResize(int size)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            270,
            "size", size);

    if (size < this->nElem) {
        for (int index = size; index < this->nElem; ++index) {
            const dsDATA_TYPE *elemType = this->pData[index].type;
            if (elemType)
                elemType->Destroy(&this->pData[index].storage);
        }
        this->nElem = size;
    }
}
