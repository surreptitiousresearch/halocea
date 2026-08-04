#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<int,8>::Realloc @ 0x823B69F0
// Resize the backing store to hold exactly `size` elements. First ShrinkResize destroys any live
// elements above the new count (int is trivially destructible, so that is just a count clamp),
// then a single dlRealloc grows/shrinks the storage in place — int is trivially relocatable, so
// there is no per-element copy/move.
template<>
void dsVECTOR<int, 8>::Realloc(int size)
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
        this->pData = (int *)dlRealloc(this->pData, 4 * size, this->__cl.file, this->__cl.line);
        this->allocated = size;
    }
}
