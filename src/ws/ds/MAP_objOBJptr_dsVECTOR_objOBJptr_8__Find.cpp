#include "MAP.h"
struct objOBJ; // boundary — map key type, used pointer-only

// ds::MAP<objOBJ*,dsVECTOR<objOBJ*,8>,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::Find(const K*)
// @ 0x82686248
// Locate `*key`'s bucket via GetHash<objOBJ*> % data.nElem, then walk the chain comparing key
// pointers directly (objOBJ* equality is pointer equality). Returns a MAP_ITERATOR whose `item`
// is null on a miss or when the table has no buckets yet.
template<>
ds::impl::MAP_ITERATOR<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>
ds::MAP<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::Find(objOBJ *const *key)
{
    ds::impl::MAP_ITERATOR<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> result;

    int nElem = this->data.nElem;
    if (nElem) {
        int bucketIndex = this->GetHash<objOBJ *>(key) % nElem;
        ITEM *node = this->data[bucketIndex];
        if (node) {
            while (node->key != *key) {
                node = node->next;
                if (!node) {
                    result.container = nullptr;
                    result.item = nullptr;
                    result.index = 0;
                    return result;
                }
            }
            result.container = this;
            result.item = node;
            result.index = bucketIndex;
            return result;
        }
    }

    result.container = nullptr;
    result.item = nullptr;
    result.index = 0;
    return result;
}
