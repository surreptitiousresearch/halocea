#include "dsVECTOR.h"
#include "../ssl/sslCLASS_REF.h"
#include "../apCL.h"

// dsVECTOR<sslCLASS_REF,8>::dsVECTOR @ 0x825C9BF8
// Empty-construct: null storage, zero count, record the call-site cookie.
// Deviation: decompiler reports "local variable allocation has failed" and emits raw register
// stores; reconstructed as the plain field-init sequence used by every other dsVECTOR(apCL) ctor.
template<>
dsVECTOR<sslCLASS_REF, 8>::dsVECTOR(const apCL &cl)
{
    this->__cl = cl;
    this->pData = nullptr;
    this->nElem = 0;
    this->allocated = 0;
}
