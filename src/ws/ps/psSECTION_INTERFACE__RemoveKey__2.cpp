#include "psSECTION_INTERFACE.h"
#include "psSECTION_DATA.h"
// @0x8251B5A0 — remove record `idx` directly (no key parsing), then drop this handle's own body
// reference if that left the section with neither records nor parents. A no-op when null.

void psSECTION_INTERFACE::RemoveKey(int idx)
{
    psSECTION_DATA *data = this->pData;
    if (data) {
        data->RemoveRec(idx);
        if (!data->orderedRecords.nElem && !data->psParents.nElem) {
            data->Release();
            this->pData = nullptr;
        }
    }
}
