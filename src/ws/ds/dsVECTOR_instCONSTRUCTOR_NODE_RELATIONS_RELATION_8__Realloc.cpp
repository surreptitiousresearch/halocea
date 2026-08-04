#include "dsVECTOR.h"
#include "../prop/instCONSTRUCTOR_NODE_RELATIONS.h"
#include "ds_assert_boundary.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<instCONSTRUCTOR_NODE_RELATIONS::RELATION,8>::Realloc @ 0x826E04B0
// Resize the backing store to hold exactly `size` elements. ShrinkResize first runs the per-slot
// string cleanup above the new count; then a single dlRealloc resizes the storage in place.
// Element size 44 bytes.
template<>
void dsVECTOR<instCONSTRUCTOR_NODE_RELATIONS::RELATION, 8>::Realloc(int size)
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
        this->pData = (instCONSTRUCTOR_NODE_RELATIONS::RELATION *)dlRealloc(
            this->pData, 44 * size, this->__cl.file, this->__cl.line);
        this->allocated = size;
    }
}
