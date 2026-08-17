/* ?GetValue@psSECTION_DATA@@QAA?AVdsDATA@@KAAUpsCOMPLEX_KEY_DATA@@@Z @0x8251CBC8 */
#include "psSECTION_DATA.h"
#include "psSECTION_INTERFACE.h"
#include "psSECTION_KEY_REF.h"
#include "psCOMPLEX_KEY_DATA.h"
#include "../ds/dsDATA.h"
#include "../ds/dsSTRID.h"
// 0x8251CBC8 — resolve the parsed `complexKey` against this section and return the located key's
// value (empty dsDATA when the key chain is empty or unresolved). Also records the resolved key's
// name and source line on psSECTION_INTERFACE's scratch globals. sret return: the decompiler swaps
// the sret slot with `this` (the real `this` is spelled `result` there).

// 0x8200155A — interned empty-string literal used to build an empty dsSTRID. boundary.
extern const char psEmptyStringLiteral[];

dsDATA psSECTION_DATA::GetValue(unsigned int flags, psCOMPLEX_KEY_DATA &complexKey)
{
    dsDATA value;
    psSECTION_KEY_REF ref;
    ref.ps.pData = nullptr;

    if (complexKey._psKeysArr.nElem == 0) {
        value.type = nullptr; // empty dsDATA
        return value;
    }

    if (this->GetValueRefs(flags, nullptr, &ref, complexKey)) {
        dsSTRID keyName;
        if (ref.ps.pData)
            keyName = ref.ps.pData->GetKeyName(ref.keyIdx);
        else
            keyName = dsSTRID(psEmptyStringLiteral, 1);

        psSECTION_INTERFACE::lastGetKeyName.id = keyName.id;
        psSECTION_INTERFACE::lastGetLine =
            ((psSECTION_INTERFACE &)ref.ps).GetKeyLineNmb(ref.keyIdx);
        value = ((psSECTION_INTERFACE &)ref.ps).GetKeyValue(ref.keyIdx);
    } else {
        value.type = nullptr; // empty dsDATA
    }

    if (ref.ps.pData)
        ref.ps.pData->Release();

    return value;
}
