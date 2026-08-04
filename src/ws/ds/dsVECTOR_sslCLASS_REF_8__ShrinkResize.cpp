#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

// ssl subsystem script class reference (element by value); destructor not reversed here.
class sslCLASS_REF { public: ~sslCLASS_REF(); }; // boundary

// dsVECTOR<sslCLASS_REF,8>::ShrinkResize @ 0x825CD4D0
// Shrink the live count to `size`, destroying every element from `size` up to the old count.
template<>
void dsVECTOR<sslCLASS_REF, 8>::ShrinkResize(int size)
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
            this->pData[i].~sslCLASS_REF();
        this->nElem = size;
    }
}
