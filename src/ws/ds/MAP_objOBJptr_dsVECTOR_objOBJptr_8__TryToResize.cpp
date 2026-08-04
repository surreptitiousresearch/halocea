#include "MAP.h"
struct objOBJ; // boundary — map key type, used pointer-only

// ds::MAP<objOBJ*,dsVECTOR<objOBJ*,8>,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::TryToResize(int)
// @ 0x8268AAD0
// Grow-on-demand policy: no buckets yet -> HashResize(17); else resize (to double the bucket
// count) once the chain-walk `depth` or the global load factor crosses escalating thresholds
// (count > 5x buckets always triggers a resize consideration; > 3x once depth > 10; > 2x once
// depth > 25), reseeding first (GenHashes) once depth > 10 before resizing. Returns whether a
// resize occurred.
template<>
bool ds::MAP<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::TryToResize(int depth)
{
    int nElem = this->data.nElem;
    if (nElem) {
        int liveCount = this->count;
        if (liveCount <= 5 * nElem) {
            if (liveCount > 3 * nElem && depth > 10) {
                this->GenHashes();
                this->HashResize(ds::FindPrime(2 * this->data.nElem));
                return true;
            }
            if (liveCount <= 2 * nElem || depth <= 25)
                return false;
            // else: count > 2x buckets and depth > 25 (but not the >3x&&>10 case above) — falls
            // through to the shared resize tail below, same as the count > 5x buckets case.
        }
        if (depth <= 10) {
            this->HashResize(ds::FindPrime(2 * this->data.nElem));
            return true;
        }
        this->GenHashes();
        this->HashResize(ds::FindPrime(2 * this->data.nElem));
        return true;
    }

    this->HashResize(17);
    return true;
}
