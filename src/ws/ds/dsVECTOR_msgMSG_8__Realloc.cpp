#include "dsVECTOR.h"
#include "ds_assert_boundary.h"
#include "../msg/msgMSG.h"

// dsVECTOR<msgMSG,8>::Realloc @ 0x82AC4D18
// Resize the backing store to hold exactly `size` elements. ShrinkResize clamps the live count,
// then _internalRealloc does the single dlRealloc of the 24-byte POD envelope array (no per-element
// copy/move — msgMSG holds no self-referencing pointers). Element size 24 bytes.
template<>
void dsVECTOR<msgMSG, 8>::Realloc(int size)
{
    if (size != this->allocated) {
        this->ShrinkResize(size);
        if (!IGNORE_STRONG_ASSERT && size < this->nElem)
            STRONG_ASSERT2_HELPER::asserd<int, int>(
                "size >= nElem",
                "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
                137,
                "size", size,
                "nElem", this->nElem);
        this->_internalRealloc(size);
        this->allocated = size;
    }
}
