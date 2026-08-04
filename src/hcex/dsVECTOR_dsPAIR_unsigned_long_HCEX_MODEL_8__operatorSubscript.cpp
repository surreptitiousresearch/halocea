#include "../headers/ws/ds/dsVECTOR.h"
#include "../headers/ws/ds/dsPAIR.h"
#include "../headers/hcex/HCEX_MODEL.h"
#include "../headers/ws/ds/ds_assert_boundary.h"

// dsVECTOR<dsPAIR<unsigned long,HCEX_MODEL>,8>::operator[] (non-const) @ 0x823DAC58
// Bounds-checked mutable access into the global hcexModels table; returns a reference to the
// idx-th (modelKey, HCEX_MODEL) pair.
typedef dsPAIR<unsigned long, HCEX_MODEL> hcex_model_pair;

template<>
hcex_model_pair &dsVECTOR<hcex_model_pair, 8>::operator[](int idx)
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
