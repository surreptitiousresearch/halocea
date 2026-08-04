#include "dsVECTOR.h"
#include "dsAFFIX_BUILDER.h"
#include "ds_assert_boundary.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<dsAFFIX_BUILDER::AFF_PARAM,8>::Realloc @ 0x82A76790
// Resize the backing store to hold exactly `size` elements. ShrinkResize clamps the live count,
// then a single dlRealloc resizes the storage in place — AFF_PARAM is a trivially relocatable
// {name,value} pair, so there is no per-element copy/move. Element size 8 bytes.
// Note: DB scopes the element as dsAFFIX_BUILDER::AFF_PARAM; the existing header models it as
// dsAFFIX_BUILDER::AFF_PARAM, so we reference that nested spelling to reuse the header.
template<>
void dsVECTOR<dsAFFIX_BUILDER::AFF_PARAM, 8>::Realloc(int size)
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
        this->pData = (dsAFFIX_BUILDER::AFF_PARAM *)dlRealloc(this->pData, 8 * size, this->__cl.file, this->__cl.line);
        this->allocated = size;
    }
}
