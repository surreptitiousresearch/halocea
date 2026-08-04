#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

// boundary — ds::MAP<objOBJ*,dsVECTOR<objOBJ*,8>,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::ITEM,
// a hash-map key/value entry record of the object-vector map. Opaque here; the vector element
// is a pointer. Distinct tag avoids ODR collision with the map's DUMMY node record.
struct dsObjOBJVecMapITEM;

// dsVECTOR<...objOBJ-map ITEM*,8>::ShrinkResize @ 0x82685D08
// Shrink the live count to `size`, dropping the trailing element slots (no reallocation).
// The element type is a pointer, so the per-slot destructor loop in the original is a no-op
// and is elided here.
template<>
void dsVECTOR<dsObjOBJVecMapITEM *, 8>::ShrinkResize(int size)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            270,
            "size", size);
    if (size < this->nElem)
        this->nElem = size;
}
