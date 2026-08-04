#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

// boundary — dsLF_SORTED_MAP<const char*,int,dsSTR_HASH,dsSTR_CMP>::LINE, a hash-chain
// node of the int-valued string map. Opaque here; the vector element is a pointer to it.
struct dsStrIntMapLINE;

// dsVECTOR<dsStrIntMapLINE*,8>::operator[] (non-const) @ 0x826165A8
// Bounds-checked mutable access; returns a reference to the idx-th element pointer.
template<>
dsStrIntMapLINE *&dsVECTOR<dsStrIntMapLINE *, 8>::operator[](int idx)
{
    if (!IGNORE_STRONG_ASSERT && (idx < 0 || idx >= this->nElem))
        STRONG_ASSERT2_HELPER::asserd<int, int>(
            "idx >= 0 && idx < nElem",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            567,
            "idx", idx,
            "nElem", this->nElem);
    return this->pData[idx];
}
