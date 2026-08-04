#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

// rend subsystem software-renderer lamp/light data record (element by value); destructor not
// reversed here. Trivially destructible, so the emitted destructor loop has an empty body.
class rendSR_LAMP_DATA { public: ~rendSR_LAMP_DATA(); }; // boundary

// dsVECTOR<rendSR_LAMP_DATA,8>::ShrinkResize @ 0x8252D248
// Shrink the live count to `size`, destroying every element from `size` up to the old count.
template<>
void dsVECTOR<rendSR_LAMP_DATA, 8>::ShrinkResize(int size)
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
            this->pData[i].~rendSR_LAMP_DATA();
        this->nElem = size;
    }
}
