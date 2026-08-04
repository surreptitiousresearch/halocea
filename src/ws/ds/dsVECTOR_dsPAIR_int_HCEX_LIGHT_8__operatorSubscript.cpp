#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "../../hcex/hcex_light.h" // reuse the real HCEX_LIGHT layout
#include "ds_assert_boundary.h"

// dsVECTOR<dsPAIR<int,HCEX_LIGHT>,8>::operator[] (non-const) @ 0x823DAB28
// Bounds-checked mutable access into the flashlight-light table; returns a reference to the idx-th
// {id,HCEX_LIGHT} pair. Target of the hcex bridge's flattened extern hcexLights_At.
template<>
dsPAIR<int, HCEX_LIGHT> &dsVECTOR<dsPAIR<int, HCEX_LIGHT>, 8>::operator[](int idx)
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
