#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

// boundary — ds::MAP<objOBJ*,dsVECTOR<objOBJ*,8>,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::ITEM,
// a hash-map key/value entry of the object-vector map. Opaque here; the element is a pointer.
// Distinct tag avoids ODR collision with the map's DUMMY node record.
struct dsObjOBJVecMapITEM;

// dsVECTOR<...objOBJ-map ITEM*,8>::Resize(int, const T&) @ 0x82689250
// Resize the live count to `size`, copy-filling any newly exposed pointer slots with `val`.
// Growing past capacity reallocates first (saving `val` in case it aliases the old storage).
// Shrinking just drops the trailing slots (pointer elements are trivially destructible).
template<>
void dsVECTOR<dsObjOBJVecMapITEM *, 8>::Resize(int size, dsObjOBJVecMapITEM *const &val)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            238,
            "size",
            size);

    int nElem = this->nElem;
    if (size < nElem) {
        this->nElem = size; // shrink: trivial per-slot destructor loop elided
        return;
    }
    if (size <= nElem) {
        this->nElem = size;
        return;
    }
    if (size <= this->allocated) {
        for (int index = nElem; index < size; ++index)
            this->pData[index] = val;
        this->nElem = size;
        return;
    }

    dsObjOBJVecMapITEM *fill = val; // hold across the reallocation
    this->Realloc(size);
    for (int index = this->nElem; index < size; ++index)
        this->pData[index] = fill;
    this->nElem = size;
}
