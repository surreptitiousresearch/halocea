/* ?GetSubSection@psSECTION_DATA@@IAAPAV1@H@Z @0x82519010 */
#include "psSECTION_DATA.h"
#include "psSECTION_RECORD.h"
#include "psSECTION.h"
#include "../ds/dsDATA.h"
#include "../ds/dsDATA_TYPE.h"
#include "../ds/ds_boundary.h"
// 0x82519010 — the sub-section body reachable through record `idx`: valid only when that record
// holds a psSECTION value whose body is directly owned by THIS section (its pOwner is `this`).
// Protected (IAA). Returns null for non-section records or foreign-owned sections.

psSECTION_DATA *psSECTION_DATA::GetSubSection(int idx)
{
    psSECTION_RECORD *record = &this->orderedRecords[idx]; // operator[] returns T& (decompiler read as ptr)
    const dsDATA_TYPE *valueType = record->value.type;
    if (valueType != dsDATA_TYPE_STORAGE<psSECTION>::dataType)
        return nullptr;

    // GetPtr yields a pointer to the stored psSECTION handle (valueType is non-null here).
    psSECTION *storedHandle = (psSECTION *)valueType->GetPtr(&record->value.storage);
    psSECTION_DATA *subSection = storedHandle->pData;
    if (!subSection)
        return nullptr;

    return (subSection->pOwner == this) ? subSection : nullptr;
}
