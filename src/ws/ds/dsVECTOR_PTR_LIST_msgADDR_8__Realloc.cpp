#include "dsVECTOR.h"
#include "PTR_LIST.h"
#include "ds_assert_boundary.h"

struct msgADDR; // boundary — msg-system endpoint address (msg subsystem)

// dsVECTOR<ds::PTR_LIST<msgADDR>,8>::Realloc @ 0x82559DF0
// Resize the backing store to hold exactly `size` elements. ShrinkResize clamps the live count,
// then _internalRealloc does the single dlRealloc of the array — each element is a PTR_LIST head
// record (pHead/pTail/length, 12 bytes) with no self-referencing pointers, trivially relocatable.
template<>
void dsVECTOR<ds::PTR_LIST<msgADDR>, 8>::Realloc(int size)
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
