#include "dsVECTOR.h"
#include "ds_assert_boundary.h"
#include "../../headers/hcex/HCEX_DECAL.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<HCEX_DECAL,8>::Realloc @ 0x823DBE00
// Resize the backing store to hold exactly `size` elements. ShrinkResize destroys any live
// elements above the new count (dropping each name's dsTSTRING buffer reference), then a single
// dlRealloc resizes the storage in place — HCEX_DECAL is treated as trivially relocatable here.
// Element size 16 bytes.
template<>
void dsVECTOR<HCEX_DECAL, 8>::Realloc(int size)
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
        this->pData = (HCEX_DECAL *)dlRealloc(this->pData, 16 * size, this->__cl.file, this->__cl.line);
        this->allocated = size;
    }
}
