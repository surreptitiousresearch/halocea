#include "psSECTION_INTERFACE.h"
#include "psSECTION_DATA.h"
#include "psCOMPLEX_KEY_DATA.h"
// 0x8251C138 — remove the (possibly dotted) key named by `key`: split it into a complex-key chain,
// remove it via psSECTION_DATA::RemoveKey, then drop this handle's own body reference if that left
// the section with neither records nor parents.

int psSECTION_INTERFACE::RemoveKey(const char *key, unsigned int flags)
{
    if (!this->pData)
        return 0;

    psCOMPLEX_KEY_DATA c;
    if (!SplitComplexKey(key, 1, 0, c) || !this->pData->RemoveKey(flags, c))
        return 0;

    psSECTION_DATA *data = this->pData;
    if (!data->orderedRecords.nElem && !data->psParents.nElem) {
        data->Release();
        this->pData = nullptr;
    }
    return 1;
}
