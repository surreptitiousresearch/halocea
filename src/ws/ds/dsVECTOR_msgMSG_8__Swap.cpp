#include "dsVECTOR.h"
#include "../msg/msgMSG.h"

// dsVECTOR<msgMSG,8>::Swap @ 0x82AC3B78
template<>
void dsVECTOR<msgMSG, 8>::Swap(dsVECTOR<msgMSG, 8> &other)
{
    msgMSG *pData = this->pData;
    this->pData = other.pData;
    other.pData = pData;

    int nElem = this->nElem;
    this->nElem = other.nElem;
    other.nElem = nElem;

    int allocated = this->allocated;
    this->allocated = other.allocated;
    other.allocated = allocated;
}
