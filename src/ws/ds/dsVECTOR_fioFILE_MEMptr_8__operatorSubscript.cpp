#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

struct fioFILE_MEM; // boundary — fio in-memory file; element is a pointer.

// dsVECTOR<fioFILE_MEM*,8>::operator[] (non-const) @ 0x82AC7E98
// Bounds-checked mutable access; returns a reference to the idx-th element pointer.
template<>
fioFILE_MEM *&dsVECTOR<fioFILE_MEM *, 8>::operator[](int idx)
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
