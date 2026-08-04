#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

#include "dsMsgAddrMapITEM.h"

// dsVECTOR<...msgADDR-map ITEM*,8>::Realloc @ 0x82559D38
// Resize the backing store to hold exactly `size` pointer elements. ShrinkResize clamps the live
// count (the element is a raw pointer, trivially destructible), then _internalRealloc does the
// single dlRealloc of the pointer array. Element size 4 bytes.
template<>
void dsVECTOR<dsMsgAddrMapITEM *, 8>::Realloc(int size)
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
