#include "dsVECTOR.h"
#include "dsDATA.h"
#include "ds_assert_boundary.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<dsDATA,8>::Realloc @ 0x825A8AB0
// Resize the backing store to hold exactly `size` elements. ShrinkResize first destroys any live
// elements above `size` (dsDATA::ShrinkResize dispatches each element type's Destroy), then a
// single dlRealloc resizes the storage in place — dsDATA is trivially relocatable (raw
// storage/type-descriptor words). Element size 8 bytes.
template<>
void dsVECTOR<dsDATA, 8>::Realloc(int size)
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
        this->pData = (dsDATA *)dlRealloc(this->pData, 8 * size, this->__cl.file, this->__cl.line);
        this->allocated = size;
    }
}
