/* ?RemoveRec@psSECTION_DATA@@QAAXH@Z @0x8251AD20 */
#include "psSECTION_DATA.h"
#include "psSECTION_INTERFACE.h"
#include "psSECTION_RECORD.h"
#include "psSECTION.h"
#include "../ds/dsDATA.h"
#include "../ds/dsDATA_TYPE.h"
#include "../ds/dsCONVERTION_TYPE.h"
#include "../ds/ds_boundary.h"
// 0x8251AD20 — remove record `idx`: snapshot its value, erase it, renumber the remaining records'
// sort predecessors, mark the table sort-dirty, and — when the removed value was a sub-section this
// section owned — detach that sub-section's owner. Finally destroy the snapshot.

void psSECTION_DATA::RemoveRec(int idx)
{
    dsDATA removedValue;
    removedValue.type = nullptr;
    if (idx >= 0 && idx < this->orderedRecords.nElem) {
        const psSECTION_RECORD *record = &this->orderedRecords[idx]; // operator[] returns T&
        removedValue.StoreValue(record->value);
    }

    this->orderedRecords.Erase(idx, 1);
    for (int j = 0; j < this->orderedRecords.nElem; ++j)
        this->orderedRecords[j].sortedIdx = j;
    this->state |= 8u;

    const dsDATA_TYPE *valueType = removedValue.type;
    if (valueType == dsDATA_TYPE_STORAGE<psSECTION>::dataType) {
        psSECTION subHandle;
        subHandle.pData = nullptr;
        removedValue.GetValue<psSECTION>(subHandle, DSD_CONV_RETRIEVE);

        psSECTION owner = ((psSECTION_INTERFACE &)subHandle).GetOwner();
        bool ownedByThis = (this == owner.pData);
        if (owner.pData)
            owner.pData->Release();

        if (ownedByThis && subHandle.pData)
            subHandle.pData->SetOwner(nullptr);
        if (subHandle.pData)
            subHandle.pData->Release();

        valueType = removedValue.type;
    }

    if (valueType)
        valueType->Destroy(&removedValue.storage);
}
