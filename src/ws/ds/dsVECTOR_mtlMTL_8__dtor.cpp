#include "dsVECTOR.h"
#include "../mtl/mtlMTL.h"

extern "C" void dlFree(void *ptr); // boundary

// dsVECTOR<mtlMTL,8>::~dsVECTOR @ 0x82684A70
// Destroy every live element in place, then free the backing storage.
template<>
dsVECTOR<mtlMTL, 8>::~dsVECTOR()
{
    for (int index = 0; index < this->nElem; ++index)
        this->pData[index].~mtlMTL();
    dlFree(this->pData);
}
