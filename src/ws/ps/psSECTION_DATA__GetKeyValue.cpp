/* ?GetKeyValue@psSECTION_DATA@@QBA?AVdsDATA@@H@Z @0x82518F48 */
#include "psSECTION_DATA.h"
#include "psSECTION_RECORD.h"
#include "../ds/dsDATA.h"
// 0x82518F48 — value stored at record index `idx`; an empty dsDATA when out of range.
// sret return: the decompiler swaps the sret slot with `this` (the real `this` is the section body,
// spelled `result` there; `result[4]` == orderedRecords and `result[4].type` == orderedRecords.nElem).

dsDATA psSECTION_DATA::GetKeyValue(int idx) const
{
    dsDATA value;
    value.type = nullptr; // raw: only the type slot (offset 4) is zeroed for the empty case
    if (idx >= 0 && idx < this->orderedRecords.nElem)
        value.StoreValue(this->orderedRecords[idx].value);
    return value;
}
