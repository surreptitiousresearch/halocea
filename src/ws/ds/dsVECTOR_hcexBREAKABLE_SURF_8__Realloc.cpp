#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

// hcex bridge breakable-surface record (element by value). Only a pointer to the element array is
// handled here, so a forward declaration suffices; its size is 100 bytes per the DB (bsp@0,
// idx@4, pInst@8, cd@12 (scnINST_CREATE_DATA, 88 bytes)), matching the 100*size below.
struct hcexBREAKABLE_SURF; // boundary

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<hcexBREAKABLE_SURF,8>::Realloc @ 0x823B2000
// Resize the backing store to hold exactly `size` elements. ShrinkResize first runs the per-slot
// destructors above the new count; then a single dlRealloc resizes the storage. Even though the
// 100-byte element has a non-trivial destructor (its embedded scnINST_CREATE_DATA strings), the
// container treats it as trivially relocatable here — the surviving elements are moved by the raw
// dlRealloc with no per-element copy/move construction.
template<>
void dsVECTOR<hcexBREAKABLE_SURF, 8>::Realloc(int size)
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
        this->pData = (hcexBREAKABLE_SURF *)dlRealloc(this->pData, 100 * size, this->__cl.file, this->__cl.line);
        this->allocated = size;
    }
}
