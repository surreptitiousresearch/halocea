#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

class rendSR_LAMP_DATA; // boundary — rend subsystem software-renderer lamp/light record (4-byte value element)

// dsVECTOR<rendSR_LAMP_DATA,8>::Realloc @ 0x8252D930
// Resize the backing store to hold exactly `size` elements. ShrinkResize clamps the live count,
// then a single dlRealloc resizes the storage in place — rendSR_LAMP_DATA is a trivially
// relocatable 4-byte record, so there is no per-element copy/move. Element size 4 bytes.
template<>
void dsVECTOR<rendSR_LAMP_DATA, 8>::Realloc(int size)
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
        this->pData = (rendSR_LAMP_DATA *)dlRealloc(this->pData, 4 * size, this->__cl.file, this->__cl.line);
        this->allocated = size;
    }
}
