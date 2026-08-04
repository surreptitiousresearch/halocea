#include "dsVECTOR.h"
#include "dsTSTRING.h"
#include "ds_assert_boundary.h"

// dsVECTOR<dsTSTRING<char>,8>::Back @ 0x825BCEE0 (const overload)
template<>
const dsTSTRING<char> &dsVECTOR<dsTSTRING<char>, 8>::Back() const
{
    if (!IGNORE_STRONG_ASSERT && this->nElem <= 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "nElem > 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            517,
            "nElem",
            this->nElem);
    return this->pData[this->nElem - 1];
}
