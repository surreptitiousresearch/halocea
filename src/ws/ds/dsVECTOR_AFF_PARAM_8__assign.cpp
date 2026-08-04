#include "dsVECTOR.h"
#include "dsAFFIX_BUILDER.h"

// dsVECTOR<dsAFFIX_BUILDER::AFF_PARAM,8>::operator= @ 0x82A776E0
// Copy-assign the affix-parameter vector: unless self-assignment (same backing store), clear
// the current elements, re-fit capacity, then splice in a copy of the source elements.
template<>
dsVECTOR<dsAFFIX_BUILDER::AFF_PARAM, 8> &
dsVECTOR<dsAFFIX_BUILDER::AFF_PARAM, 8>::operator=(
    const dsVECTOR<dsAFFIX_BUILDER::AFF_PARAM, 8> &src)
{
    if (src.pData != this->pData) {
        this->Clear();
        // DB-verified: Realloc receives this->nElem (== 0 after Clear), not src.nElem.
        if (this->allocated != src.nElem)
            this->Realloc(this->nElem);
        this->Insert(0, src.pData, src.nElem);
    }
    return *this;
}
