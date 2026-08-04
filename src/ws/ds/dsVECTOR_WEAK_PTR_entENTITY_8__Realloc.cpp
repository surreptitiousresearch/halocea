#include "dsVECTOR.h"
#include "WEAK_PTR.h"
#include "ds_assert_boundary.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

struct entENTITY; // boundary — ent entity; referenced only through the weak handle.

// dsVECTOR<ds::WEAK_PTR<entENTITY>,8>::Realloc @ 0x823DBA80
// Resize the backing store to hold exactly `size` elements. ShrinkResize releases/nulls any live
// weak refs above the new count, then a single dlRealloc resizes the storage in place — the weak
// ref is a single handle pointer, trivially relocatable, so there is no per-element copy/move.
// Element size 4 bytes.
template<>
void dsVECTOR<ds::WEAK_PTR<entENTITY>, 8>::Realloc(int size)
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
        this->pData = (ds::WEAK_PTR<entENTITY> *)dlRealloc(this->pData, 4 * size, this->__cl.file, this->__cl.line);
        this->allocated = size;
    }
}
