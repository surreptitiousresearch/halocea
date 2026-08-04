#include "dsVECTOR.h"
#include "../msg/saLIST.h"
#include "ds_assert_boundary.h"

// dsVECTOR<saLIST,8>::ShrinkResize @ 0x82AC6500
// Shrink the live count to `size`, destroying every element's addList from `size` up to the old
// count (freeing its cells).
template<>
void dsVECTOR<saLIST, 8>::ShrinkResize(int size)
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
            this->pData[i].addList.Clear();
        this->nElem = size;
    }
}
