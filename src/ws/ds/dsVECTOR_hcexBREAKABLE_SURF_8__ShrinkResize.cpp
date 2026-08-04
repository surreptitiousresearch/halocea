#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

// hcex bridge breakable-surface record (element by value). Its implicit destructor only
// needs to destroy the embedded `cd` (a scnINST_CREATE_DATA) member; layout not reversed here.
struct hcexBREAKABLE_SURF { ~hcexBREAKABLE_SURF(); }; // boundary

// dsVECTOR<hcexBREAKABLE_SURF,8>::ShrinkResize @ 0x823B1E00
// Shrink the live count to `size`, destroying every element from `size` up to the old count.
template<>
void dsVECTOR<hcexBREAKABLE_SURF, 8>::ShrinkResize(int size)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            270,
            "size",
            size);
    if (size < this->nElem) {
        for (int i = size; i < this->nElem; ++i)
            this->pData[i].~hcexBREAKABLE_SURF();
        this->nElem = size;
    }
}
