#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "dsTSTRING.h"
#include "ds_assert_boundary.h"

// dsVECTOR<dsPAIR<dsTSTRING<char>,int>,8>::operator[] (non-const) @ 0x823B6440
// Bounds-checked mutable access into the object-name -> Blam-id table; returns a reference to the
// idx-th pair.
template<>
dsPAIR<dsTSTRING<char>, int> &dsVECTOR<dsPAIR<dsTSTRING<char>, int>, 8>::operator[](int idx)
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
