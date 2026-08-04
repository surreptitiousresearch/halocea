#include "dsVECTOR.h"
#include "../msg/msgMSG.h"
#include "../apCL.h"

// dsVECTOR<msgMSG,8>::dsVECTOR @ 0x825560C8
// Empty-construct: null storage, zero count, record the call-site cookie.
// Deviation: decompiler packs the apCL argument oddly (writes `this` into __cl.file and the raw
// `cl` param into __cl.line); reconstructed as the plain apCL field-copy used by every other
// dsVECTOR(apCL) ctor, since apCL is a {file,line} pair passed by value here.
template<>
dsVECTOR<msgMSG, 8>::dsVECTOR(const apCL &cl)
{
    this->pData = nullptr;
    this->nElem = 0;
    this->allocated = 0;
    this->__cl = cl;
}
