#include "dsVECTOR.h"
#include "ds_assert_boundary.h"
#include "../m3d/m3dMATR.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<m3dMATR,8>::Realloc @ 0x823DBB30
// Resize the backing store to hold exactly `size` elements. ShrinkResize clamps the live count
// (m3dMATR is trivially destructible plain matrix data), then a single dlRealloc resizes the
// storage in place — m3dMATR is trivially relocatable, so there is no per-element copy/move.
// Element size 64 bytes (disasm: size << 6).
template<>
void dsVECTOR<m3dMATR, 8>::Realloc(int size)
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
        this->pData = (m3dMATR *)dlRealloc(this->pData, 64 * size, this->__cl.file, this->__cl.line);
        this->allocated = size;
    }
}
