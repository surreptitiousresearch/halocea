#include "dsVECTOR.h"
#include "dsTSTRING.h"
#include "ds_assert_boundary.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<dsTSTRING<char>,8>::Realloc @ 0x823B5968
// Resize the backing store to hold exactly `size` elements. ShrinkResize first destroys any
// string elements above the new count (dropping their buffer references); then a single dlRealloc
// resizes the storage. dsTSTRING<char> is a single pointer (4 bytes) and is trivially
// relocatable, so the elements are moved by the raw dlRealloc with no per-element copy/move.
template<>
void dsVECTOR<dsTSTRING<char>, 8>::Realloc(int size)
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
        this->pData = (dsTSTRING<char> *)dlRealloc(this->pData, 4 * size, this->__cl.file, this->__cl.line);
        this->allocated = size;
    }
}
