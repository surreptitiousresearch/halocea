#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

// boundary — dsLF_SORTED_MAP<const char*,int,dsSTR_HASH,dsSTR_CMP>::LINE, a hash-chain
// node of the int-valued string map. Opaque here; the vector element is a pointer to it.
struct dsStrIntMapLINE;

// dsVECTOR<dsStrIntMapLINE*,8>::operator[] (const) @ 0x82616640
// Bounds-checked read-only access; returns a const reference to the idx-th element pointer.
template<>
dsStrIntMapLINE *const &dsVECTOR<dsStrIntMapLINE *, 8>::operator[](int idx) const
{
    if (!IGNORE_STRONG_ASSERT && (idx < 0 || idx >= this->nElem))
        STRONG_ASSERT2_HELPER::asserd<int, int>(
            "idx >= 0 && idx < nElem",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            557,
            "idx", idx,
            "nElem", this->nElem);
    return this->pData[idx];
}
