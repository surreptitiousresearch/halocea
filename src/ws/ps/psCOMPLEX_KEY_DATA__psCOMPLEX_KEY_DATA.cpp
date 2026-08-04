#include "psCOMPLEX_KEY_DATA.h"
#include "../ds/dsSTRID.h"
// 0x82518194 — default-construct: default-construct all 20 inline dsSTRID slots (empty-string ids)
// and leave `_psKeysArr.nElem` at 0; `_psSubStr` adopts the shared empty-string singleton.

psCOMPLEX_KEY_DATA::psCOMPLEX_KEY_DATA()
{
    _psKeysArr.nElem = 0;
    for (int i = 0; i < 20; ++i)
        _psKeysArr.pData[i] = dsSTRID();

    _psSubStr.pBuffer = nullptr;
    _psSubStr.UnsafeInitEmpty();
}
