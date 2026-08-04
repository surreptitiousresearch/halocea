#include "MAP.h"
struct msgADDR; // fwd (pointer element only)

// ds::MAP<int,msgADDR*,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::HashResize @ 0x8255C390
// Rehash every live node into a freshly-sized bucket vector, then swap it in for `data`.
//   - `size` is rounded up to the next usable prime (ds::CheckPrime/ds::FindPrime); if that prime
//     happens to equal the current seed `a` (which doubles as a hash multiplier), it's bumped again
//     to avoid a degenerate all-nodes-collide case.
//   - A temporary `data.nElem`==0-cleared bucket vector is Resize()'d to `Prime` buckets (source
//     line ds_map.hpp:289, reproduced as a debug call-site cookie on the temp), then every node in
//     every OLD bucket is unlinked and reinserted (prepended) at its NEW bucket in the temp vector.
//   - The temp vector is swapped into `data` (O(1) pointer swap) and destroyed holding the old array.
template<>
void ds::MAP<int, msgADDR *, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::HashResize(int size)
{
    int prime = size;
    if (!ds::CheckPrime(size)) {
        prime = ds::FindPrime(prime);
        if (prime == (int)a)
            prime = ds::FindPrime(prime + 17);
    }

    apCL resizeCl = { "d:\\projects\\code\\common\\incl.sys\\ds\\ds_map.hpp", 289 };
    dsVECTOR<ITEM *, 8> freshBuckets;
    freshBuckets.pData = nullptr;
    freshBuckets.nElem = 0;
    freshBuckets.allocated = 0;
    freshBuckets.__cl = resizeCl;
    freshBuckets.Resize(prime, nullptr);

    for (int i = 0; i < data.nElem; ++i) {
        if (data[i]) {
            ITEM *node = data[i];
            while (node) {
                ITEM *next = node->next;

                unsigned int newHash = GetHash<int>(&node->key);
                int newBucket = (int)(newHash % (unsigned int)freshBuckets.nElem);

                node->next = freshBuckets[newBucket];
                freshBuckets[newBucket] = node;

                node = next;
            }
        }
    }

    data.Swap(freshBuckets);
    // freshBuckets (now holding the OLD bucket array) is destroyed here via its own destructor.
}
