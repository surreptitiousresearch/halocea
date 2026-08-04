#include "dsVECTOR.h"
#include "ds_assert_boundary.h"
#include "dsPARAM.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<dsPARAM,8>::Realloc @ 0x823DB918
// Resize the backing store to hold exactly `size` elements. ShrinkResize destroys any live
// elements above the new count, then a single dlRealloc resizes the storage in place — dsPARAM is
// treated as trivially relocatable here (no per-element copy/move). Element size 12 bytes.
template<>
void dsVECTOR<dsPARAM, 8>::Realloc(int size)
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
        this->pData = (dsPARAM *)dlRealloc(this->pData, 12 * size, this->__cl.file, this->__cl.line);
        this->allocated = size;
    }
}
