/* ?RemoveKey@psSECTION_DATA@@QAAHKAAUpsCOMPLEX_KEY_DATA@@@Z @0x8251B768 */
#include "psSECTION_DATA.h"
#include "psSECTION.h"
#include "psCOMPLEX_KEY_DATA.h"
#include "../ds/dsSTATIC_VECTOR.h"
#include "../ds/dsDATA.h"
#include "../ds/dsSTRID.h"
// 0x8251B774 — remove the key named by the last segment of `c`'s key chain. For a single-segment
// chain, removes the record directly. For a multi-segment (dotted) chain, descends into the
// sub-section named by the FIRST segment (`c`'s Back(), since the chain is stored leaf-first /
// reversed) and recurses with that segment popped off.

int psSECTION_DATA::RemoveKey(unsigned int flags, psCOMPLEX_KEY_DATA &c)
{
    if (c._psKeysArr.nElem == 0)
        return 0;

    dsSTRID lastKey = c._psKeysArr.Back();
    int idx = FindRecThisOnly(lastKey);
    if (idx == -1)
        return 0;

    if (c._psKeysArr.nElem == 1) {
        RemoveRec(idx);
        return 1;
    }

    dsDATA value = GetKeyValue(idx);
    psSECTION sub(value);
    if (value.type)
        value.type->Destroy(&value.storage);

    if (!sub.pData)
        return 0;

    if (c._psKeysArr.nElem > 0)
        --c._psKeysArr.nElem;

    int result = sub.pData->RemoveKey(flags, c);
    if (sub.pData)
        sub.pData->Release();
    return result;
}
