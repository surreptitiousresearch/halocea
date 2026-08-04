#include "../headers/ws/ds/dsVECTOR.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

// dsVECTOR<haloPLAYER_ACTIONS_ENV,8>::dsVECTOR(const apCL&) @ 0x823BB700
// Empty-vector ctor: null storage, zero count/capacity, adopting the allocation call-site cookie.
// DEVIATION: the PPC decompiler renders the by-value apCL member copy as two field writes assigning
// `this` to __cl.file and `cl` to __cl.line; that is an artifact of it not modeling the apCL copy.
// Real semantics (per the dsVECTOR(const apCL&) contract): `__cl = cl`.
template<>
dsVECTOR<haloPLAYER_ACTIONS_ENV, 8>::dsVECTOR(const apCL &cl)
{
    this->pData = nullptr;
    this->nElem = 0;
    this->allocated = 0;
    this->__cl = cl;
}
