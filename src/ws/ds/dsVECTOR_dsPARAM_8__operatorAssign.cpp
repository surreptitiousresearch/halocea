#include "dsVECTOR.h"
#include "dsPARAM.h"

// dsVECTOR<dsPARAM,8>::operator= @ 0x827F1D00
template<>
dsVECTOR<dsPARAM, 8> &dsVECTOR<dsPARAM, 8>::operator=(const dsVECTOR<dsPARAM, 8> &src)
{
    if (src.pData != this->pData) {
        this->Clear();
        if (this->allocated != src.nElem)
            this->Realloc(this->nElem);
        this->Insert(0, src.pData, src.nElem);
    }
    return *this;
}
