#include "psSECTION_DATA.h"
#include "psSECTION_RECORD.h"
#include "../ds/dsSTRID.h"
// 0x82518EE0 — key name (interned) at record index `idx`; an empty dsSTRID when out of range.
// sret return: the decompiler swaps the sret slot with `this`; the record-index bound is
// orderedRecords.nElem and the value returned is the record's keyStrId.

dsSTRID psSECTION_DATA::GetKeyName(int idx) const
{
    if (idx < 0 || idx >= this->orderedRecords.nElem)
        return dsSTRID();
    return this->orderedRecords[idx].keyStrId;
}
