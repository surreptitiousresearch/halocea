/* ?GetNParent@psSECTION_INTERFACE@@QBAHXZ @0x82516998 */
#include "psSECTION_INTERFACE.h"
// 0x82516998 — parent-section count, or 0 when the handle is null.

int psSECTION_INTERFACE::GetNParent() const
{
    psSECTION_DATA *sectionBody = this->pData;
    if (sectionBody)
        return sectionBody->psParents.nElem;
    return 0;
}
