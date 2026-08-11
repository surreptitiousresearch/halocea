#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/hcex/HALO_SOUND_PARAMS.h"

// dsVECTOR<HALO_SOUND_PARAMS::REPLACE_INFO,8>::operator= @ 0x836BDCC8
// Copy-assign: unless self-assignment (same backing store), destroy the current contents
// (REPLACE_INFO is trivially destructible here, so the compiler emitted an empty per-element
// loop), drop the element count, re-fit capacity only when it differs from the source size, then
// splice in a copy of the source elements.
template<>
dsVECTOR<HALO_SOUND_PARAMS::REPLACE_INFO, 8> &
dsVECTOR<HALO_SOUND_PARAMS::REPLACE_INFO, 8>::operator=(
    const dsVECTOR<HALO_SOUND_PARAMS::REPLACE_INFO, 8> &src)
{
    if (src.pData != this->pData) {
        int oldAllocated = this->allocated;
        this->nElem = 0;
        // DB-verified: Realloc receives 0, not src.nElem.
        if (oldAllocated != src.nElem)
            this->Realloc(0);
        this->Insert(0, src.pData, src.nElem);
    }
    return *this;
}
