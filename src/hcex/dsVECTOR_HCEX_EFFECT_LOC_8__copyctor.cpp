// dsVECTOR<HCEX_EFFECT_LOC,8>::dsVECTOR(const dsVECTOR<HCEX_EFFECT_LOC,8> &src) @ 0x823E2A80
//
// Copy-construct one effect's placed-locations table: start empty (null storage, zero
// count/capacity), adopt src's allocation call-site cookie, then Insert the whole source range.
// Reversed per-instantiation.

#include "../headers/ws/ds/dsVECTOR.h"
#include "../headers/hcex/HCEX_EFFECT_LOC.h"

template<>
dsVECTOR<HCEX_EFFECT_LOC, 8>::dsVECTOR(const dsVECTOR<HCEX_EFFECT_LOC, 8> &src)
{
    this->pData = nullptr;
    this->nElem = 0;
    this->allocated = 0;
    this->__cl = src.__cl;
    this->Insert(0, src.pData, src.nElem);
}
