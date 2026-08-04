#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<char,8>::Realloc @ 0x823B6AA0
// Resize the backing store to hold exactly `size` bytes. ShrinkResize clamps the live count
// (char is trivially destructible), then a single dlRealloc resizes the storage in place —
// char is trivially relocatable, so there is no per-element copy/move. Element size 1 byte.
template<>
void dsVECTOR<char, 8>::Realloc(int size)
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
        this->pData = (char *)dlRealloc(this->pData, size, this->__cl.file, this->__cl.line);
        this->allocated = size;
    }
}
