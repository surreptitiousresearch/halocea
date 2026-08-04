#include "dsVECTOR.h"
#include "../vid/vidOBJ_SPLIT.h"
#include "ds_assert_boundary.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<vidOBJ_SPLIT,8>::Realloc @ 0x82C872A8
// Resize the backing store to hold exactly `size` elements. ShrinkResize first runs the per-slot
// cleanup above the new count; then a single dlRealloc resizes the storage in place. Element
// size 284 bytes.
template<>
void dsVECTOR<vidOBJ_SPLIT, 8>::Realloc(int size)
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
        this->pData = (vidOBJ_SPLIT *)dlRealloc(this->pData, 284 * size, this->__cl.file, this->__cl.line);
        this->allocated = size;
    }
}
