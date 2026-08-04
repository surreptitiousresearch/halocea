#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "dsTSTRING.h"
#include "ds_assert_boundary.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<dsPAIR<dsTSTRING<char>,dsTSTRING<char>>,8>::Realloc @ 0x823DBF68
// Resize the backing store to hold exactly `size` elements. ShrinkResize first drops references on
// any elements above the new count, then a single dlRealloc resizes the storage in place.
// Element size 8 bytes.
template<>
void dsVECTOR<dsPAIR<dsTSTRING<char>, dsTSTRING<char> >, 8>::Realloc(int size)
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
        this->pData = (dsPAIR<dsTSTRING<char>, dsTSTRING<char> > *)dlRealloc(this->pData, 8 * size, this->__cl.file, this->__cl.line);
        this->allocated = size;
    }
}
