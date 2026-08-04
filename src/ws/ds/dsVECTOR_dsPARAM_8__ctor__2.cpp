#include "dsVECTOR.h"
#include "dsPARAM.h"
#include "../apCL.h"

// dsVECTOR<dsPARAM,8>::dsVECTOR (copy) @ 0x827EE458
template<>
dsVECTOR<dsPARAM, 8>::dsVECTOR(const dsVECTOR<dsPARAM, 8> &src)
{
    this->pData = nullptr;
    this->nElem = 0;
    this->allocated = 0;
    this->__cl = src.__cl;
    this->Insert(0, src.pData, src.nElem);
}
