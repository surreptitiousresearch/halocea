#include "dsVECTOR.h"
#include "../m3d/m3dMATR.h"

// dsVECTOR<m3dMATR,8>::Length @ 0x82B2BDF8
template<>
int dsVECTOR<m3dMATR, 8>::Length() const
{
    return this->nElem;
}
