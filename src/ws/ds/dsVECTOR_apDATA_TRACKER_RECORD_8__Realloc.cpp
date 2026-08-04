#include "dsVECTOR.h"
#include "../ap/apDATA_TRACKER.h"
#include "ds_assert_boundary.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<apDATA_TRACKER::RECORD,8>::Realloc @ 0x82A24CF0
// Resize the backing store to hold exactly `size` elements. ShrinkResize clamps the live count
// (destroying each dropped RECORD's dsTSTRING name), then a single dlRealloc resizes the storage
// in place — RECORD is trivially relocatable, so there is no per-element copy/move.
// Element size 124 bytes (disasm: 124 * size).
template<>
void dsVECTOR<apDATA_TRACKER::RECORD, 8>::Realloc(int size)
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
        this->pData = (apDATA_TRACKER::RECORD *)dlRealloc(this->pData, 124 * size, this->__cl.file, this->__cl.line);
        this->allocated = size;
    }
}
