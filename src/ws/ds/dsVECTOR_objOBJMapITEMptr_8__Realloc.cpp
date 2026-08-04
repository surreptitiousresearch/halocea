#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// boundary — ds::MAP<objOBJ*,dsVECTOR<objOBJ*,8>,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::ITEM,
// a hash-map key/value entry record of the object-vector map. Opaque here; the vector element
// is a pointer. Distinct tag avoids ODR collision with the map's DUMMY node record.
struct dsObjOBJVecMapITEM;

// dsVECTOR<...objOBJ-map ITEM*,8>::Realloc @ 0x82686A20
// Resize the backing store to hold exactly `size` elements. ShrinkResize clamps the live count
// (the element is a raw pointer, trivially destructible), then a single dlRealloc resizes the
// storage in place — a pointer is trivially relocatable, so there is no per-element copy/move.
template<>
void dsVECTOR<dsObjOBJVecMapITEM *, 8>::Realloc(int size)
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
        this->pData = (dsObjOBJVecMapITEM **)dlRealloc(this->pData, 4 * size, this->__cl.file, this->__cl.line);
        this->allocated = size;
    }
}
