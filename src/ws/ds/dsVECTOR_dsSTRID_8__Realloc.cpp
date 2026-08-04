#include "dsVECTOR.h"
#include "dsSTRID.h"
#include "ds_assert_boundary.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<dsSTRID,8>::Realloc @ 0x823DB568
// Resize the backing store to hold exactly `size` elements. ShrinkResize clamps the live count
// (dsSTRID is a trivially destructible interned-id pointer), then a single dlRealloc resizes the
// storage in place — dsSTRID is trivially relocatable, so there is no per-element copy/move.
// Element size 4 bytes.
template<>
void dsVECTOR<dsSTRID, 8>::Realloc(int size)
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
        this->pData = (dsSTRID *)dlRealloc(this->pData, 4 * size, this->__cl.file, this->__cl.line);
        this->allocated = size;
    }
}
