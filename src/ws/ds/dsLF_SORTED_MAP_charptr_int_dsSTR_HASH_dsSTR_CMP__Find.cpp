#include "dsLF_SORTED_MAP.h"

struct dsSTR_HASH; // boundary — const char* hashing policy
struct dsSTR_CMP;  // boundary — const char* ordering policy

// dsLF_SORTED_MAP<const char*,int,dsSTR_HASH,dsSTR_CMP>::Find @ 0x82616D48
// Register the thread with the deferred-erase manager, hash the key to a bucket, then binary
// search that bucket's chain node (FindKey) for *key. Returns a pointer to the stored int
// value, or null if absent. const method (QBA mangle).
template<>
const int *dsLF_SORTED_MAP<const char *, int, dsSTR_HASH, dsSTR_CMP>::Find(
    const char *const *key) const
{
    dsERASE_MANAGER<LINE_ERASER> *eraseMgr = GetEraseMgr();
    eraseMgr->CheckThread();

    int nBuckets = this->hashTable.nElem;
    int hash = this->GetHash(key);
    // hashTable is mutable state of the map even in this const lookup; cast away const to reach
    // the non-const operator[] the build inlines here.
    dsVECTOR<LINE *, 8> &buckets = const_cast<dsVECTOR<LINE *, 8> &>(this->hashTable);
    LINE *line = buckets[hash % nBuckets];

    int itemIndex = this->FindKey(line, key);
    if (itemIndex >= 0)
        return &line->items[itemIndex].val;
    return nullptr;
}
