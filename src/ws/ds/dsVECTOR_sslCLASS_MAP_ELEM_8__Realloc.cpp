#include "dsVECTOR.h"
#include "../ssl/sslCLASS_MAP_ELEM.h"
#include "ds_assert_boundary.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<sslCLASS_MAP_ELEM,8>::Realloc @ 0x825CF680
// Resize the backing store to hold exactly `size` elements. ShrinkResize first runs the per-slot
// cls cleanup above the new count; then a single dlRealloc resizes the storage in place (the
// container treats the 8-byte element as trivially relocatable here — surviving elements are
// moved by the raw reallocation, no per-element copy/move construction). Element size 8 bytes.
template<>
void dsVECTOR<sslCLASS_MAP_ELEM, 8>::Realloc(int size)
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
        this->pData = (sslCLASS_MAP_ELEM *)dlRealloc(this->pData, 8 * size, this->__cl.file, this->__cl.line);
        this->allocated = size;
    }
}
