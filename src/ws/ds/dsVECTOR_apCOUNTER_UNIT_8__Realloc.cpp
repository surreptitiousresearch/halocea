#include "dsVECTOR.h"
#include "ds_assert_boundary.h"
#include "../ap/apCOUNTER_UNIT.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<apCOUNTER_UNIT,8>::Realloc @ 0x827156C0
// Resize the backing store to hold exactly `size` elements. ShrinkResize destroys any live
// elements above the new count (dropping each apCOUNTER_UNIT's name-buffer reference), then a
// single dlRealloc resizes the storage in place — apCOUNTER_UNIT is trivially relocatable here, so
// there is no per-element copy/move. Element size 48 bytes.
template<>
void dsVECTOR<apCOUNTER_UNIT, 8>::Realloc(int size)
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
        this->pData = (apCOUNTER_UNIT *)dlRealloc(this->pData, 48 * size, this->__cl.file, this->__cl.line);
        this->allocated = size;
    }
}
