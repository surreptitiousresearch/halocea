#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

struct strmVID_BUFFER; // boundary — strm streaming video buffer; element is a pointer.

// dsVECTOR<strmVID_BUFFER*,8>::Realloc @ 0x826990B0
// Resize the backing store to hold exactly `size` elements. ShrinkResize clamps the live count,
// then a single dlRealloc resizes the storage in place — a pointer is trivially relocatable, so
// there is no per-element copy/move. Element size 4 bytes.
template<>
void dsVECTOR<strmVID_BUFFER *, 8>::Realloc(int size)
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
        this->pData = (strmVID_BUFFER **)dlRealloc(this->pData, 4 * size, this->__cl.file, this->__cl.line);
        this->allocated = size;
    }
}
