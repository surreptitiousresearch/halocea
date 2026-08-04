#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "ds_assert_boundary.h"
#include "../scn/scnINST_CREATE_DATA.h"

// dsVECTOR<dsPAIR<unsigned long,scnINST_CREATE_DATA>,8>::ShrinkResize @ 0x823D9738
// Shrink the live count to `size`, destroying each element's scnINST_CREATE_DATA value above the
// new count. The unsigned-long key is trivial.
template<>
void dsVECTOR<dsPAIR<unsigned long, scnINST_CREATE_DATA>, 8>::ShrinkResize(int size)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            270,
            "size", size);

    if (size < this->nElem) {
        for (int index = size; index < this->nElem; ++index)
            this->pData[index].val.~scnINST_CREATE_DATA();
        this->nElem = size;
    }
}
