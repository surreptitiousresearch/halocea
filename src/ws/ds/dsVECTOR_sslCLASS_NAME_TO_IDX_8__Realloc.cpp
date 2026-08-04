#include "dsVECTOR.h"
#include "../ssl/sslCLASS_NAME_TO_IDX.h"
#include "ds_assert_boundary.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<sslCLASS::NAME_TO_IDX,8>::Realloc @ 0x82A3A758
// Resize the backing store to hold exactly `size` elements. ShrinkResize clamps the live count,
// then a single dlRealloc resizes the storage in place. Element size 8 bytes.
template<>
void dsVECTOR<ssl_class::NAME_TO_IDX, 8>::Realloc(int size)
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
        this->pData = (ssl_class::NAME_TO_IDX *)dlRealloc(this->pData, 8 * size, this->__cl.file, this->__cl.line);
        this->allocated = size;
    }
}
