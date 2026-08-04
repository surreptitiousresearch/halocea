#include "dsVECTOR.h"
#include "ds_assert_boundary.h"
#include "dsStrStridMapERASED.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<dsERASE_MANAGER<...dsSTRID-map LINE_ERASER>::ERASED,8>::Realloc @ 0x8252A7A0
// Resize the backing store to hold exactly `size` elements. ShrinkResize clamps the live count
// (destroying the trimmed ERASED records, a trivial POD teardown), then a single dlRealloc resizes
// the storage in place — ERASED is trivially relocatable. Element size 12 bytes.
template<>
void dsVECTOR<dsStrStridMapERASED, 8>::Realloc(int size)
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
        this->pData = (dsStrStridMapERASED *)dlRealloc(this->pData, 12 * size, this->__cl.file, this->__cl.line);
        this->allocated = size;
    }
}
