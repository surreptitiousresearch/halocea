#include "dsVECTOR.h"
#include "../m3d/m3dMATR.h"

// dsVECTOR<m3dMATR,8>::Clear @ 0x823ED650
// m3dMATR is a plain POD — no per-element teardown, just reset the count.
template<>
void dsVECTOR<m3dMATR, 8>::Clear()
{
    this->nElem = 0;
}
