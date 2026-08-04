#include "../headers/hcex/haloENGINE_CONTROL.h"
#include "../headers/ws/ds/dsVECTOR.h"
#include "../headers/ws/ds/dsPAIR.h"
#include "../headers/ws/ds/dsPARAM_LIST.h"
#include "../headers/ws/ds/ds_assert_boundary.h"

// dsVECTOR<dsPAIR<int, void (haloENGINE_CONTROL::*)(unsigned long, const dsPARAM_LIST &)>, 8>
//     ::operator[] (non-const) @ 0x823C8AD8
// Bounds-checked mutable access into haloENGINE_CONTROL's (event-id -> handler member-fn) map.
typedef dsPAIR<int, void (haloENGINE_CONTROL::*)(unsigned long, const dsPARAM_LIST &)> EventMapEntry;

template<>
EventMapEntry &dsVECTOR<EventMapEntry, 8>::operator[](int idx)
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
