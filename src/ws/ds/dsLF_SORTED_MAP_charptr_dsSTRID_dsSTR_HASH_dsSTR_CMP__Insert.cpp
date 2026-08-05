#include "dsLF_SORTED_MAP.h"
#include "dsSTR_HASH.h"
#include "dsSTRID.h"
#include "ds_assert_boundary.h"

struct dsSTR_CMP; // boundary — const char* ordering policy

extern "C" void osMemoryBarrier(); // boundary — full memory fence

// dsLF_SORTED_MAP<const char*,dsSTRID,dsSTR_HASH,dsSTR_CMP>::Insert @ 0x8252B5D0
// Insert key->val into its hash bucket, keeping the bucket chain key-sorted. Registers the thread
// with the deferred-erase manager, hashes to a bucket, finds the lower-bound slot in that node,
// asserts the key is not already present, builds the {key,val} item, splices it into a fresh node
// via InsertItem, and publishes the new node back into the bucket behind a memory barrier.
template<>
void dsLF_SORTED_MAP<const char *, dsSTRID, dsSTR_HASH, dsSTR_CMP>::Insert(
    const char *const &key, const dsSTRID &val)
{
    GetEraseMgr()->CheckThread();

    int bucketCount = this->hashTable.nElem;
    int bucket = this->GetHash(&key) % bucketCount;
    LINE *line = this->hashTable[bucket];
    int index = this->FindKeyGE(line, &key);

    if (!IGNORE_STRONG_ASSERT && line && index != line->length && line->items[index].key == key)
        ((STRONG_ASSERT_DUMMY *)nullptr)->Crash(
            "line == 0 || index == line->length || line->items[index].key != key",
            "D:\\Projects\\code\\common\\incl.sys\\ds\\ds_lf_map.h",
            261,
            empty_string);

    ITEM item;
    item.key = key;
    item.val.id = val.id;

    LINE *inserted = this->InsertItem(line, index, &item);
    osMemoryBarrier();
    this->hashTable[bucket] = inserted;
}
