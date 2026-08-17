/* ?IsEmptyHier@psSECTION_INTERFACE@@QBAHXZ @0x8251B390 */
#include "psSECTION_INTERFACE.h"
#include "psSECTION.h"
#include "psSECTION_DATA.h"
// 0x8251B390 — true when this section and its entire parent hierarchy hold no keys. A null handle
// is empty; otherwise every parent must be empty-hier AND this section must have no records. const.

int psSECTION_INTERFACE::IsEmptyHier() const
{
    if (!this->pData)
        return 1;

    for (int i = 0; i < this->GetNParent(); ++i) {
        psSECTION parentHandle = this->GetParent(i);
        bool parentNotEmpty = (((psSECTION_INTERFACE &)parentHandle).IsEmptyHier() == 0);
        if (parentHandle.pData)
            parentHandle.pData->Release();
        if (parentNotEmpty)
            return 0;
    }

    return this->pData->orderedRecords.nElem == 0;
}
