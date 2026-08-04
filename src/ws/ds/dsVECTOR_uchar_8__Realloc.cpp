#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<unsigned char,8>::Realloc @ 0x825AEFE0
// Resize the backing store to hold exactly `size` elements. ShrinkResize clamps the live count
// (unsigned char is trivially destructible), then a single dlRealloc resizes the storage in place.
// Element size 1 byte, so the byte count passed to dlRealloc is `size`.
template<>
void dsVECTOR<unsigned char, 8>::Realloc(int size)
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
        this->pData = (unsigned char *)dlRealloc(this->pData, size, this->__cl.file, this->__cl.line);
        this->allocated = size;
    }
}
