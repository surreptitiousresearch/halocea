#include "../headers/ws/ds/dsVECTOR.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"
#include "../headers/ws/ds/ds_assert_boundary.h"

// dsVECTOR<haloPLAYER_ACTIONS_ENV,8>::operator[] (non-const) @ 0x823BFD70
// Bounds-checked mutable access; returns a reference to the idx-th element.
// The trailing a3..a14 params the PPC decompiler invents are vararg-slot artifacts of the
// STRONG_ASSERT2 helper call; the real signature takes only `idx`.
template<>
haloPLAYER_ACTIONS_ENV &dsVECTOR<haloPLAYER_ACTIONS_ENV, 8>::operator[](int idx)
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
