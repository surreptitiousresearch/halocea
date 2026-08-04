#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

struct psSECTION_RECORD; // boundary — ps (particle system) section record (element by value, 20 bytes)

// dsVECTOR<psSECTION_RECORD,8>::Realloc @ 0x82518918
// Resize the backing store to hold exactly `size` elements. ShrinkResize clamps the live count,
// then a single dlRealloc resizes the storage in place — psSECTION_RECORD is trivially relocatable,
// so there is no per-element copy/move. Element size 20 bytes.
template<>
void dsVECTOR<psSECTION_RECORD, 8>::Realloc(int size)
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
        this->pData = (psSECTION_RECORD *)dlRealloc(this->pData, 20 * size, this->__cl.file, this->__cl.line);
        this->allocated = size;
    }
}
