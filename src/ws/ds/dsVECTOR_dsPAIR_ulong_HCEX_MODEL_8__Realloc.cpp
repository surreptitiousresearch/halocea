#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "ds_assert_boundary.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

#include "HCEX_MODEL.h"

// dsVECTOR<dsPAIR<unsigned long,HCEX_MODEL>,8>::Realloc @ 0x823E0600
// Resize the backing store to hold exactly `size` elements. ShrinkResize clamps the live count,
// then a single dlRealloc resizes the storage in place — the pair is treated as trivially
// relocatable here, so there is no per-element copy/move. Element size 76 bytes.
template<>
void dsVECTOR<dsPAIR<unsigned long, HCEX_MODEL>, 8>::Realloc(int size)
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
        this->pData = (dsPAIR<unsigned long, HCEX_MODEL> *)dlRealloc(this->pData, 76 * size, this->__cl.file, this->__cl.line);
        this->allocated = size;
    }
}
