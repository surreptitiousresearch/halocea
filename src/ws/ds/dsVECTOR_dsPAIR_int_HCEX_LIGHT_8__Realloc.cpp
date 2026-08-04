#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "ds_assert_boundary.h"
#include "../../hcex/hcex_light.h" // reuse the real HCEX_LIGHT layout

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<dsPAIR<int,HCEX_LIGHT>,8>::Realloc @ 0x823DC0C8
// Resize the backing store to hold exactly `size` elements. ShrinkResize clamps the live count
// (the pair holds trivially destructible POD), then a single dlRealloc resizes the storage in
// place — the pair is trivially relocatable, so there is no per-element copy/move.
// Element size 16 bytes.
template<>
void dsVECTOR<dsPAIR<int, HCEX_LIGHT>, 8>::Realloc(int size)
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
        this->pData = (dsPAIR<int, HCEX_LIGHT> *)dlRealloc(this->pData, 16 * size, this->__cl.file, this->__cl.line);
        this->allocated = size;
    }
}
