#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<short,8>::Realloc @ 0x823DB618
// Resize the backing store to hold exactly `size` elements. ShrinkResize clamps the live count
// (short is trivially destructible), then a single dlRealloc resizes the storage in place —
// short is trivially relocatable, so there is no per-element copy/move. Element size 2 bytes.
template<>
void dsVECTOR<short, 8>::Realloc(int size)
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
        this->pData = (short *)dlRealloc(this->pData, 2 * size, this->__cl.file, this->__cl.line);
        this->allocated = size;
    }
}
