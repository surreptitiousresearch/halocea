#include "dsVECTOR.h"
#include "../ssl/sslCLASS_MAP_ELEM.h"
#include "ds_assert_boundary.h"

// dsVECTOR<sslCLASS_MAP_ELEM,8>::ShrinkResize @ 0x825CD270
// Shrink the live count to `size`, destroying every element's cls ref from `size` up to the old
// count.
template<>
void dsVECTOR<sslCLASS_MAP_ELEM, 8>::ShrinkResize(int size)
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
            this->pData[i].cls.~sslCLASS_REF();
        this->nElem = size;
    }
}
