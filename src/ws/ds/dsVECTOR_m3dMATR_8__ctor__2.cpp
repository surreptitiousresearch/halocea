#include "dsVECTOR.h"
#include "../m3d/m3dMATR.h"
#include "../apCL.h"

// dsVECTOR<m3dMATR,8>::dsVECTOR (copy) @ 0x823EEB28
template<>
dsVECTOR<m3dMATR, 8>::dsVECTOR(const dsVECTOR<m3dMATR, 8> &src)
{
    this->pData = nullptr;
    this->nElem = 0;
    this->allocated = 0;
    this->__cl = src.__cl;
    this->Insert(0, src.pData, src.nElem);
}
