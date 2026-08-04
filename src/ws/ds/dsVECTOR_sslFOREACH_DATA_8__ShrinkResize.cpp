#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

// ssl subsystem script for-each iterator record (element by value). Its implicit
// destructor only needs to destroy the embedded `checkClass` (a sslCLASS_REF) member;
// layout/members not reversed here.
struct sslFOREACH_DATA { ~sslFOREACH_DATA(); }; // boundary

// dsVECTOR<sslFOREACH_DATA,8>::ShrinkResize @ 0x82AF1550
// Shrink the live count to `size`, destroying every element from `size` up to the old count.
template<>
void dsVECTOR<sslFOREACH_DATA, 8>::ShrinkResize(int size)
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
            this->pData[i].~sslFOREACH_DATA();
        this->nElem = size;
    }
}
