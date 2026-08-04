#include "dsVECTOR.h"
#include "../mtl/mtlMTL.h"

// dsVECTOR<mtlMTL,8>::dsVECTOR(const apCL&) @ 0x82680E50
// Construct an empty vector, adopting the allocation call-site cookie `cl`.
template<>
dsVECTOR<mtlMTL, 8>::dsVECTOR(const apCL &cl)
{
    this->__cl = cl;
    this->pData = 0;
    this->nElem = 0;
    this->allocated = 0;
}
