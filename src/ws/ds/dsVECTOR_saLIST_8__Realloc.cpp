#include "dsVECTOR.h"
#include "../msg/saLIST.h"
#include "ds_assert_boundary.h"

// dsVECTOR<saLIST,8>::Realloc @ 0x82AC6A80
// Resize the backing store to hold exactly `size` elements. ShrinkResize first runs the per-slot
// addList cleanup above the new count; then a single _internalRealloc resizes the storage (the
// container treats the 24-byte element as trivially relocatable here — surviving elements are
// moved by the raw reallocation, no per-element copy/move construction).
template<>
void dsVECTOR<saLIST, 8>::Realloc(int size)
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
