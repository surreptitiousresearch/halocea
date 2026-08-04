#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "dsTSTRING.h"
#include "ds_assert_boundary.h"

// dsVECTOR<dsPAIR<int,dsTSTRING<char>>,8>::Back @ 0x82AA2028
// Reference to the last live element (index nElem-1); assert non-empty first.
template<>
dsPAIR<int, dsTSTRING<char> > &dsVECTOR<dsPAIR<int, dsTSTRING<char> >, 8>::Back()
{
    if (!IGNORE_STRONG_ASSERT && this->nElem <= 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "nElem > 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            527,
            "nElem",
            this->nElem);
    return this->pData[this->nElem - 1];
}
