/* ?GetKeyValue@psSECTION_INTERFACE@@QBA?AVdsDATA@@H@Z @0x825194D0 */
#include "psSECTION_INTERFACE.h"
#include "../ds/dsDATA.h"
// 0x825194D0 — key value at record index `idx`, forwarding to the body; an empty dsDATA when the
// handle is null. sret return: the decompiler swaps the sret slot with `this`.

dsDATA psSECTION_INTERFACE::GetKeyValue(int idx) const
{
    if (this->pData)
        return this->pData->GetKeyValue(idx);

    dsDATA emptyValue;
    emptyValue.type = nullptr; // raw: only the type slot (offset 4) is zeroed for the empty case
    return emptyValue;
}
