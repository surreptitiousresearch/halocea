#include "dsVECTOR.h"
#include "ds_assert_boundary.h"
#include "../../headers/hcex/HCEX_EFFECT_LOC.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<HCEX_EFFECT_LOC,8>::Realloc @ 0x823DBC98
// Resize the backing store to hold exactly `size` elements. ShrinkResize destroys any live
// elements above the new count (dropping each name's dsTSTRING buffer reference), then a single
// dlRealloc resizes the storage in place — HCEX_EFFECT_LOC is treated as trivially relocatable
// here (the buffer-header pointer inside its dsTSTRING moves verbatim). Element size 28 bytes.
template<>
void dsVECTOR<HCEX_EFFECT_LOC, 8>::Realloc(int size)
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
        this->pData = (HCEX_EFFECT_LOC *)dlRealloc(this->pData, 28 * size, this->__cl.file, this->__cl.line);
        this->allocated = size;
    }
}
