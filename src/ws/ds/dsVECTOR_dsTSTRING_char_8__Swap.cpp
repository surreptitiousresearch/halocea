#include "dsVECTOR.h"
#include "dsTSTRING.h"

// dsVECTOR<dsTSTRING<char>,8>::Swap @ 0x82CCDB70
// Exchange the three backing-storage fields (pData/nElem/allocated) with `other`.
template<>
void dsVECTOR<dsTSTRING<char>, 8>::Swap(dsVECTOR<dsTSTRING<char>, 8> &other)
{
    dsTSTRING<char> *pData = this->pData;
    this->pData = other.pData;
    other.pData = pData;

    int nElem = this->nElem;
    this->nElem = other.nElem;
    other.nElem = nElem;

    int allocated = this->allocated;
    this->allocated = other.allocated;
    other.allocated = allocated;
}
