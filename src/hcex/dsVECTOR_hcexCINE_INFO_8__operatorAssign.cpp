// dsVECTOR<hcexCINE_INFO,8>::operator=(const dsVECTOR<hcexCINE_INFO,8> &src) @ 0x823B7FE0
//
// Copy-assign the per-difficulty (actor,mask) table: self-assignment guard on the storage pointer,
// clear the live count (hcexCINE_INFO is trivially destructible — the decompiler's per-element
// destroy loop is empty), drop the backing store when the capacity doesn't already match the
// source length, then Insert the whole source range. Reversed per-instantiation.
//
// DEVIATION: the mangled name returns `const dsVECTOR&`; the primary-template declaration returns a
// non-const `dsVECTOR&`. Matched to the declared signature so the specialization binds; semantics
// (return *this) are unchanged.

#include "../headers/hcex/hcexCINE_INFO.h"
#include "../headers/ws/ds/dsVECTOR.h"

template<>
dsVECTOR<hcexCINE_INFO, 8> &dsVECTOR<hcexCINE_INFO, 8>::operator=(const dsVECTOR<hcexCINE_INFO, 8> &src)
{
    if (src.pData != this->pData)
    {
        this->nElem = 0;
        if (this->allocated != src.nElem)
            this->Realloc(0);
        this->Insert(0, src.pData, src.nElem);
    }
    return *this;
}
