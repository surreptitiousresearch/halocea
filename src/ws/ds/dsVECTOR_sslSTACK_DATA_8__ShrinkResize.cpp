#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

// ssl subsystem script stack record (element by value); destructor not reversed here.
class sslSTACK_DATA { public: ~sslSTACK_DATA(); }; // boundary

// dsVECTOR<sslSTACK_DATA,8>::ShrinkResize @ 0x82AD92D0
// Shrink the live count to `size`, destroying every element from `size` up to the old count.
template<>
void dsVECTOR<sslSTACK_DATA, 8>::ShrinkResize(int size)
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
            this->pData[i].~sslSTACK_DATA();
        this->nElem = size;
    }
}
