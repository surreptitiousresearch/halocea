// dsVECTOR<hcexCINE_INFO,8>::dsVECTOR(const dsVECTOR<hcexCINE_INFO,8> &src) @ 0x823B7F80
//
// Copy-construct the per-difficulty (actor,mask) table: start empty (null storage, zero
// count/capacity), adopt src's allocation call-site cookie, then Insert the whole source range.
// Reversed per-instantiation.

#include "../headers/hcex/hcexCINE_INFO.h"
#include "../headers/ws/ds/dsVECTOR.h"

template<>
dsVECTOR<hcexCINE_INFO, 8>::dsVECTOR(const dsVECTOR<hcexCINE_INFO, 8> &src)
{
    this->pData = nullptr;
    this->nElem = 0;
    this->allocated = 0;
    this->__cl = src.__cl;
    this->Insert(0, src.pData, src.nElem);
}
