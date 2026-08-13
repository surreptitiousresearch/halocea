#include "psSECTION_DATA.h"
#include <string.h>
#include "psSECTION.h"
#include "psSECTION_BRANCH.h"
#include "psSECTION_KEY_REF.h"
#include "psCOMPLEX_KEY_DATA.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsSTATIC_VECTOR.h"
#include "../ds/dsSTRID.h"
#include "../ds/dsDATA.h"
#include "../ds/dsDATA_TYPE.h"
#include "../ds/ds_boundary.h"
#include "psGET_FLAGS.h"

// 0x8251C860 — resolve the parsed complex key `c` against this section, collecting located key
// references. With an empty chain, this section itself is a reference. Otherwise the leaf key (or,
// when it is the empty-string sentinel, a sub-string prefix match) is looked up in this section's
// records; if the value there is a sub-section the search descends into it with the leaf key popped.
// After the local search, parent sections (unless state&1) and the owner scope (unless state&2 or
// flags&4) are searched. When `pRefArr` is non-null every match is appended for a collect-all query;
// when `pOneRef` is non-null the first match is written and a single-ref query returns 1 early.

// 0x8200155A — interned empty-string literal (the sub-string-prefix sentinel leaf key). boundary.
extern const char psEmptyStringLiteral[];

int psSECTION_DATA::GetValueRefs(unsigned int flags, dsVECTOR<psSECTION, 8> *pRefArr,
                                 psSECTION_KEY_REF *pOneRef, psCOMPLEX_KEY_DATA &c)
{
    // The 0x40000000 (searching-owner) and 0x1 flags suppress the direct record lookup in this
    // section: skip straight to the parent/owner walk.
    if ((flags & 0x40000001) == 0) {
        if (c._psKeysArr.nElem == 0) {
            // Empty chain: this section itself is a value reference. Append a referenced handle.
            if (pRefArr) {
                psSECTION selfHandle;
                selfHandle.pData = this;
                if (this) {
                    ++this->refCount;
                    ++this->pBranch->refCount;
                }
                pRefArr->PushBack(selfHandle);
                if (this)
                    this->Release(); // PushBack copied its own reference; drop the manual add-ref
            }
            goto parent_owner_walk;
        }

        int recIdx;
        if (c._psKeysArr.nElem == 1 && c._psKeysArr.Back() == psEmptyStringLiteral) {
            // Leaf key is the empty-string sentinel: find the last record whose name begins with the
            // stored sub-string prefix.
            recIdx = this->orderedRecords.nElem - 1;
            if (recIdx < 0)
                goto parent_owner_walk;
            for (;;) {
                dsSTRID keyName = this->GetKeyName(recIdx);
                const char *keyStr = keyName.CStr();
                if (strstr(keyStr, c._psSubStr.pBuffer->str) == keyStr)
                    break;
                if (--recIdx < 0)
                    goto parent_owner_walk;
            }
        } else {
            recIdx = this->FindRecThisOnly(c._psKeysArr.Back());
        }

        if (recIdx >= 0) {
            if (c._psKeysArr.nElem == 1) {
                if (pOneRef) {
                    psSECTION selfHandle;
                    selfHandle.pData = this;
                    if (this) {
                        ++this->refCount;
                        ++this->pBranch->refCount;
                    }
                    pOneRef->ps = selfHandle;   // referenced assignment (adds its own reference)
                    if (this)
                        this->Release();        // balance the manual add-ref
                    pOneRef->keyIdx = recIdx;
                    pOneRef = nullptr;
                }
                if (!pRefArr)
                    return 1;
            }

            // If the located record's value is itself a sub-section, descend into it.
            dsDATA leafValue = this->GetKeyValue(recIdx);
            bool leafIsSection = (leafValue.type == dsDATA_TYPE_STORAGE<psSECTION>::dataType);
            if (leafValue.type)
                leafValue.type->Destroy(&leafValue.storage);

            if (leafIsSection) {
                dsDATA subValue = this->GetKeyValue(recIdx);
                psSECTION subSection(subValue);
                if (subValue.type)
                    subValue.type->Destroy(&subValue.storage);

                dsSTRID poppedKey = c._psKeysArr.Back();
                if (c._psKeysArr.nElem > 0)
                    --c._psKeysArr.nElem; // pop the leaf key before recursing

                if (subSection.pData->GetValueRefs(flags, pRefArr, pOneRef, c)) {
                    if (subSection.pData)
                        subSection.pData->Release();
                    return 1;
                }

                c._psKeysArr.PushBack(poppedKey); // restore the popped key
                if (subSection.pData)
                    subSection.pData->Release();
            }
        }
    }

parent_owner_walk:
    if ((flags & PS_GET_NO_PARENT) == 0) {
        if ((this->state & 1) == 0) {
            // Search parent sections newest-first, clearing the owner (0x40000000) and 0x1 flags.
            for (int parentIdx = this->psParents.nElem - 1; parentIdx >= 0; --parentIdx) {
                psSECTION &parent = this->psParents[parentIdx];
                if (parent.pData->GetValueRefs(flags & ~0x40000001u, pRefArr, pOneRef, c))
                    return 1;
            }
        }
        if ((this->state & 2) == 0 && (flags & PS_GET_NO_OWNER) == 0 && this->pOwner) {
            // Prepend this section's name to the chain and search the owner scope.
            c._psKeysArr.PushBack(this->name);
            if (!this->pOwner->GetValueRefs(flags | _PS_GET_NO_THIS_SYS, pRefArr, pOneRef, c)) {
                if (c._psKeysArr.nElem > 0)
                    --c._psKeysArr.nElem;
                return 0;
            }
            return 1;
        }
    }
    return 0;
}
