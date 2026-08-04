#include "dsVECTOR.h"
#include "dsPARAM.h"
#include "../apCL.h"

// dsVECTOR<dsPARAM,8>::dsVECTOR @ 0x823CF7D8
// Empty-construct: null storage, zero count, record the call-site cookie.
// Deviation: decompiler reports "local variable allocation has failed" and emits raw register
// stores; reconstructed as the plain field-init sequence used by every other dsVECTOR(apCL) ctor.
template<>
dsVECTOR<dsPARAM, 8>::dsVECTOR(const apCL &cl)
{
    this->__cl = cl;
    this->pData = nullptr;
    this->nElem = 0;
    this->allocated = 0;
}
