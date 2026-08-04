#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "ds_assert_boundary.h"
#include "../scn/scnINST_CREATE_DATA.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<dsPAIR<unsigned long,scnINST_CREATE_DATA>,8>::Realloc @ 0x823DC018
// Resize the backing store to hold exactly `size` elements. ShrinkResize first destroys any
// elements above the new count, then a single dlRealloc resizes the storage in place (the pair is
// treated as trivially relocatable here). Element size 92 bytes.
template<>
void dsVECTOR<dsPAIR<unsigned long, scnINST_CREATE_DATA>, 8>::Realloc(int size)
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
        this->pData = (dsPAIR<unsigned long, scnINST_CREATE_DATA> *)dlRealloc(this->pData, 92 * size, this->__cl.file, this->__cl.line);
        this->allocated = size;
    }
}
