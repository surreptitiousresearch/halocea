#include "psSECTION_INTERFACE.h"
#include "psSECTION.h"
// 0x8251A410 — adopt `psOwner` as this section's owner. If this handle is empty it first creates an
// empty body (unless the owner is also empty, in which case there is nothing to do). The by-value
// owner handle's reference is released after the body records it.

void psSECTION_INTERFACE::SetOwner(psSECTION psOwner)
{
    if (!this->pData)
    {
        if (!psOwner.pData)
            return;
        this->CreateEmpty();
    }

    this->pData->SetOwner(psOwner.pData);
    if (psOwner.pData)
        psOwner.pData->Release();
}
