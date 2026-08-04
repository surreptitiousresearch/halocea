#include "dsVECTOR.h"
#include "../m3d/m3dMATR.h"

// dsVECTOR<m3dMATR,8>::operator= @ 0x823EEA28
// m3dMATR is a plain POD — the copy is a Clear + full re-Insert (no per-element assignment path).
template<>
dsVECTOR<m3dMATR, 8> &dsVECTOR<m3dMATR, 8>::operator=(const dsVECTOR<m3dMATR, 8> &src)
{
    if (src.pData != this->pData) {
        this->nElem = 0;
        if (this->allocated != src.nElem)
            this->Realloc(0);
        this->Insert(0, src.pData, src.nElem);
    }
    return *this;
}
