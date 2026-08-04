#include "dsVECTOR.h"

class psSECTION; // boundary — ps subsystem section handle (element by value)

// dsVECTOR<psSECTION,8>::dsVECTOR(const apCL &) @ 0x82516520
// Construct an empty vector: null storage, zero count, adopt the allocation call-site cookie.
template<>
dsVECTOR<psSECTION, 8>::dsVECTOR(const apCL &cl)
{
    this->__cl = cl;
    this->pData = nullptr;
    this->nElem = 0;
    this->allocated = 0;
}
