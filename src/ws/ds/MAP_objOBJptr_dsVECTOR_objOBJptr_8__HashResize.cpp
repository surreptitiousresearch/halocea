#include "MAP.h"
struct objOBJ; // boundary — map key type, used pointer-only

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// ds::MAP<objOBJ*,dsVECTOR<objOBJ*,8>,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::HashResize(int)
// @ 0x8268A398
// Rehash every live node into a freshly-sized bucket vector: round `size` up to a usable prime
// (bumping again if it would collide with the current seed `a`), rehash each chain into the new
// vector by re-walking every existing bucket and re-linking each node at `GetHash(key) % newSize`,
// then swap the new vector in for `data` and free the old backing storage.
template<>
void ds::MAP<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::HashResize(int size)
{
    int newSize = size;
    if (!ds::CheckPrime(size)) {
        newSize = ds::FindPrime(size);
        if (newSize == static_cast<int>(this->a))
            newSize = ds::FindPrime(newSize + 17);
    }

    ITEM *nullFill = nullptr;
    dsVECTOR<ITEM *, 8> newBuckets;
    newBuckets.pData = nullptr;
    newBuckets.nElem = 0;
    newBuckets.allocated = 0;
    newBuckets.__cl.file = "d:\\projects\\code\\common\\incl.sys\\ds\\ds_map.hpp";
    newBuckets.__cl.line = 289;
    newBuckets.Resize(newSize, nullFill);

    int newBucketCount = newBuckets.nElem;
    ITEM **newBucketArray = newBuckets.pData;

    int oldBucketCount = this->data.nElem;
    for (int oldBucket = 0; oldBucket < oldBucketCount; ++oldBucket) {
        ITEM *node = this->data[oldBucket];
        if (!node)
            continue;
        while (node) {
            ITEM *next = node->next;
            int newBucket = this->GetHash<objOBJ *>(&node->key) % newBucketCount;
            node->next = newBucketArray[newBucket];
            newBucketArray[newBucket] = node;
            node = next;
        }
    }

    ITEM **oldData = this->data.pData;
    this->data.pData     = newBuckets.pData;
    this->data.nElem     = newBuckets.nElem;
    this->data.allocated = newBuckets.allocated;
    dlFree(oldData);
}
