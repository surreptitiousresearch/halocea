#include "psSECTION_INTERFACE.h"
// 0x825169B0 — key/record count, or 0 when the handle is null.

int psSECTION_INTERFACE::GetNKey() const
{
    psSECTION_DATA *sectionBody = this->pData;
    if (sectionBody)
        return sectionBody->orderedRecords.nElem;
    return 0;
}
