#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "dsTSTRING.h"
#include "ds_assert_boundary.h"

// dsVECTOR<dsPAIR<dsTSTRING<char>,dsTSTRING<char>>,8>::operator[] (non-const) @ 0x823D9FB8
// Bounds-checked mutable access into a string-key/string-value pair table; returns a reference to
// the idx-th pair.
template<>
dsPAIR<dsTSTRING<char>, dsTSTRING<char> > &
dsVECTOR<dsPAIR<dsTSTRING<char>, dsTSTRING<char> >, 8>::operator[](int idx)
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
