#include "psSECTION_DATA.h"
#include "psSECTION_INTERFACE.h"
#include "psSECTION_RECORD.h"
#include "psSECTION.h"
#include "psSECTION_BRANCH.h"
#include "../ds/dsDATA.h"
#include "../ds/dsDATA_TYPE.h"
#include "../ds/dsSTRID.h"
#include "../ds/dsCONVERTION_TYPE.h"
#include "../ds/ds_boundary.h"
// 0x8251B868 — append a record {`key`, `obj`} at source line `lineNmb`. Unless `key` is the empty
// key, an existing record for `key` either aborts (-1) or, with (flags&1), is removed first. Pushes
// the new record, marks the table sort-dirty, and — for sub-section values — makes this section the
// child's owner and names it. Returns the new record index or -1. Protected (IAA); consumes `obj`.

// 0x8200155A — interned empty-string literal used to build an empty dsSTRID. boundary.
extern const char psEmptyStringLiteral[];

int psSECTION_DATA::AddRec(dsSTRID key, dsDATA obj, unsigned int flags, int lineNmb)
{
    dsSTRID emptyKey(psEmptyStringLiteral, 1);

    if (key.id != emptyKey.id) {
        int existing = this->FindRecThisOnly(key);
        if (existing != -1) {
            if (!(flags & 1)) {
                // Key present and not replacing: reject, consuming `obj`.
                if (obj.type)
                    obj.type->Destroy(&obj.storage);
                return -1;
            }
            this->RemoveRec(existing);
        }
    }

    psSECTION_RECORD record;
    record.keyStrId = dsSTRID();
    record.keyStrId.id = key.id;
    record.value.type = nullptr;
    record.value.StoreValue(obj);
    record.sortedIdx = this->orderedRecords.nElem;
    record.lineNmb = (short)lineNmb;
    this->orderedRecords.PushBack(record);
    this->state |= 8u;

    if (obj.type == dsDATA_TYPE_STORAGE<psSECTION>::dataType) {
        psSECTION subHandle;
        subHandle.pData = nullptr;
        obj.GetValue<psSECTION>(subHandle, DSD_CONV_RETRIEVE);

        // Pre-reference this section (and its branch): SetOwner consumes the by-value owner handle.
        psSECTION_BRANCH *branch = this->pBranch;
        ++this->refCount;
        ++branch->refCount;

        psSECTION selfHandle;
        selfHandle.pData = this;
        ((psSECTION_INTERFACE &)subHandle).SetOwner(selfHandle);
        subHandle.pData->name.id = key.id;
        if (subHandle.pData)
            subHandle.pData->Release();
    }

    int newIdx = this->orderedRecords.nElem - 1;

    // Destroy the stack record's value copy and consume the by-value `obj`.
    if (record.value.type)
        record.value.type->Destroy(&record.value.storage);
    if (obj.type)
        obj.type->Destroy(&obj.storage);

    return newIdx;
}
