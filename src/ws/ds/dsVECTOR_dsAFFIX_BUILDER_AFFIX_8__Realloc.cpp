#include "dsVECTOR.h"
#include "dsAFFIX_BUILDER.h"
#include "ds_assert_boundary.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<dsAFFIX_BUILDER::AFFIX,8>::Realloc @ 0x82A76840
// Resize the backing store to hold exactly `size` elements. ShrinkResize destroys any live AFFIX
// records above the new count (releasing each affix's params vector and name buffer), then a single
// dlRealloc resizes the storage in place — AFFIX is treated as trivially relocatable, so there is
// no per-element copy/move. Element size 24 bytes.
template<>
void dsVECTOR<dsAFFIX_BUILDER::AFFIX, 8>::Realloc(int size)
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
        this->pData = (dsAFFIX_BUILDER::AFFIX *)dlRealloc(this->pData, 24 * size, this->__cl.file, this->__cl.line);
        this->allocated = size;
    }
}
