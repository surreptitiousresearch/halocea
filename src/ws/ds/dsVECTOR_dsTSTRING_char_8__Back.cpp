#include "dsVECTOR.h"
#include "dsTSTRING.h"
#include "ds_assert_boundary.h"

// dsVECTOR<dsTSTRING<char>,8>::Back @ 0x823B56F8
// Reference to the last live element (index nElem-1); assert the vector is non-empty first.
template<>
dsTSTRING<char> &dsVECTOR<dsTSTRING<char>, 8>::Back()
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
