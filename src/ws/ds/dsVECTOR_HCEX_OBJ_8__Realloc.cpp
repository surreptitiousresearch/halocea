#include "dsVECTOR.h"
#include "ds_assert_boundary.h"
#include "../../headers/hcex/HCEX_OBJ.h" // reuse the real HCEX_OBJ layout

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<HCEX_OBJ,8>::Realloc @ 0x823EE258
// Resize the backing store to hold exactly `size` elements. ShrinkResize destroys any live
// elements above the new count (freeing each HCEX_OBJ's inner follower vectors), then a single
// dlRealloc resizes the storage in place — HCEX_OBJ is treated as trivially relocatable here
// (its inner vector/pointer members move verbatim). Element size 56 bytes.
template<>
void dsVECTOR<HCEX_OBJ, 8>::Realloc(int size)
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
        this->pData = (HCEX_OBJ *)dlRealloc(this->pData, 56 * size, this->__cl.file, this->__cl.line);
        this->allocated = size;
    }
}
