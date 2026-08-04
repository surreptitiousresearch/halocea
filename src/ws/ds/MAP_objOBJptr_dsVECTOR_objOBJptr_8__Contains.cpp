#include "MAP.h"
struct objOBJ; // boundary — map key type, used pointer-only

// ds::MAP<objOBJ*,dsVECTOR<objOBJ*,8>,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::Contains(const K*,V*)
// const @ 0x82689F30
// True if `*key` is present (bucket lookup + chain walk by pointer equality); when `value` is
// non-null, also copies out the matched value.
template<>
bool ds::MAP<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::Contains(
    objOBJ *const *key, dsVECTOR<objOBJ *, 8> *value) const
{
    int nElem = this->data.nElem;
    if (!nElem)
        return false;

    int bucketIndex = this->GetHash<objOBJ *>(key) % nElem;
    ITEM *node = this->data[bucketIndex];
    if (!node)
        return false;

    while (node->key != *key) {
        node = node->next;
        if (!node)
            return false;
    }

    if (value)
        *value = node->value;
    return true;
}
