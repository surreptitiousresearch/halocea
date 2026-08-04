#include "../headers/hcex/haloENGINE_CONTROL.h"

// dsVECTOR<dsPAIR<int, void (haloENGINE_CONTROL::*)(unsigned long, const dsPARAM_LIST &)>, 8>::dsVECTOR(
//     const apCL &cl) @ 0x823C1BB0
//
// Empty-vector ctor for haloENGINE_CONTROL::GetEventMap()'s backing store: null storage, zero
// count/capacity, adopting the allocation call-site cookie `cl` by value.
// DEVIATION: the PPC decompiler renders the apCL member copy as two field writes assigning `this`
// to __cl.file and `cl` to __cl.line; that is an artifact of it not modeling the by-value apCL copy.
// Real semantics (per the dsVECTOR(const apCL&) contract): `__cl = cl`.
typedef dsPAIR<int, void (haloENGINE_CONTROL::*)(unsigned long, const dsPARAM_LIST &)> EventMapEntry;

template<>
dsVECTOR<EventMapEntry, 8>::dsVECTOR(const apCL &cl)
{
    this->pData = nullptr;
    this->nElem = 0;
    this->allocated = 0;
    this->__cl = cl;
}
