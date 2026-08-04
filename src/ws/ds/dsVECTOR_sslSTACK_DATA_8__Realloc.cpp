#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

struct sslSTACK_DATA; // boundary — ssl VM stack slot; element is a value.

// dsVECTOR<sslSTACK_DATA,8>::Realloc @ 0x82AF1718
// Resize the backing store to hold exactly `size` elements. ShrinkResize clamps the live count,
// then a single dlRealloc resizes the storage in place — sslSTACK_DATA is trivially relocatable,
// so there is no per-element copy/move. Element size 16 bytes (disasm: 16 * size).
template<>
void dsVECTOR<sslSTACK_DATA, 8>::Realloc(int size)
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
        this->pData = (sslSTACK_DATA *)dlRealloc(this->pData, 16 * size, this->__cl.file, this->__cl.line);
        this->allocated = size;
    }
}
