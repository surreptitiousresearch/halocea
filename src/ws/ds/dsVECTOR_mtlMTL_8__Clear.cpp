#include "dsVECTOR.h"
#include "../mtl/mtlMTL.h"

// dsVECTOR<mtlMTL,8>::Clear @ 0x82684458
// Destroy every live element in place, then reset the element count to zero.
template<>
void dsVECTOR<mtlMTL, 8>::Clear()
{
    for (int index = 0; index < this->nElem; ++index)
        this->pData[index].~mtlMTL();
    this->nElem = 0;
}
