#include "dsVECTOR.h"
#include "ds_assert_boundary.h"
#include "../../headers/hcex/HCEX_MODEL_REGION.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<HCEX_MODEL_REGION,8>::Realloc @ 0x823DBBE0
// Resize the backing store to hold exactly `size` elements. ShrinkResize destroys any live
// elements above the new count, then a single dlRealloc resizes the storage in place —
// HCEX_MODEL_REGION is treated as trivially relocatable here (no per-element copy/move).
// Element size 48 bytes.
template<>
void dsVECTOR<HCEX_MODEL_REGION, 8>::Realloc(int size)
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
        this->pData = (HCEX_MODEL_REGION *)dlRealloc(this->pData, 48 * size, this->__cl.file, this->__cl.line);
        this->allocated = size;
    }
}
