// dsVECTOR<HCEX_MODEL_REGION,8>::dsVECTOR(const dsVECTOR<HCEX_MODEL_REGION,8> &src) @ 0x823E5290
//
// Copy-construct the region table of an HCEX_MODEL: start empty (null storage, zero count/capacity),
// adopt src's allocation call-site cookie, then Insert the whole source range (Insert deep-copies
// each HCEX_MODEL_REGION via its copy ctor). Reversed per-instantiation.

#include "../headers/hcex/HCEX_MODEL_REGION.h"
#include "../headers/ws/ds/dsVECTOR.h"

template<>
dsVECTOR<HCEX_MODEL_REGION, 8>::dsVECTOR(const dsVECTOR<HCEX_MODEL_REGION, 8> &src)
{
    this->pData = nullptr;
    this->nElem = 0;
    this->allocated = 0;
    this->__cl = src.__cl;
    this->Insert(0, src.pData, src.nElem);
}
