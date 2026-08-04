#include "dsLF_SORTED_MAP.h"
#include "dsSTRID.h"

// dsSTR_HASH / dsSTR_CMP — string hash/compare policies for the const-char* keyed map.
struct dsSTR_HASH; // boundary
struct dsSTR_CMP;  // boundary

// dsLF_SORTED_MAP<const char*,dsSTRID,dsSTR_HASH,dsSTR_CMP>::Find @ 0x8252A488
// Look up a string key: register the thread with the deferred-erase manager, hash into the
// bucket chain, and binary-search the chain node for the key. Returns a pointer to the stored
// dsSTRID value, or null when the key is absent.
template<>
const dsSTRID *dsLF_SORTED_MAP<const char *, dsSTRID, dsSTR_HASH, dsSTR_CMP>::Find(
    const char *const *key) const
{
    dsERASE_MANAGER<LINE_ERASER> *eraseMgr = GetEraseMgr();
    eraseMgr->CheckThread();

    int bucketCount = this->hashTable.nElem;
    int hash = this->GetHash(key);
    const LINE *bucketNode = this->hashTable[hash % bucketCount];

    int itemIndex = this->FindKey(bucketNode, key);
    if (itemIndex >= 0)
        return &bucketNode->items[itemIndex].val;
    return nullptr;
}
