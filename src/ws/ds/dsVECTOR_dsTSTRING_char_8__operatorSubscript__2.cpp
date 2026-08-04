#include "dsVECTOR.h"
#include "dsTSTRING.h"
#include "ds_assert_boundary.h"

// dsVECTOR<dsTSTRING<char>,8>::operator[] @ 0x823C8C18 (const overload)
template<>
const dsTSTRING<char> &dsVECTOR<dsTSTRING<char>, 8>::operator[](int index) const
{
    if (!IGNORE_STRONG_ASSERT && (index < 0 || index >= this->nElem))
        STRONG_ASSERT2_HELPER::asserd<int, int>(
            "idx >= 0 && idx < nElem",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            557,
            "idx", index, "nElem", this->nElem);
    return this->pData[index];
}
