#include "MAP.h"
struct msgADDR; // fwd (pointer element only)

// ds::MAP<int,msgADDR*,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::TryToResize @ 0x8255C590
// Grow-on-demand policy, called by Insert right after linking a new node with `depth` = that
// node's position in its bucket chain (1-based):
//   - No buckets allocated yet: HashResize(17); always resizes.
//   - Otherwise: no resize needed while `count` stays under an escalating set of load-factor caps
//     relative to `data.nElem` (5x always; 3x once the chain got 10+ deep; 2x once 25+ deep) — i.e.
//     a single abnormally-long chain forces a resize even if the GLOBAL load factor is still low.
//   - Past those caps: reseed the hash once depth > 10 (a pathologically long chain suggests a bad
//     seed, not just growth), then double the bucket count (FindPrime(2*nElem)) and resize.
// Returns whether a resize actually happened, so Insert knows its just-linked node's bucket index
// may now be stale.
template<>
bool ds::MAP<int, msgADDR *, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::TryToResize(int depth)
{
    if (!data.nElem) {
        HashResize(17);
        return true;
    }

    if (count <= 5 * data.nElem
        && (count <= 3 * data.nElem || depth <= 10)
        && (count <= 2 * data.nElem || depth <= 25)) {
        return false;
    }

    if (depth > 10)
        GenHashes();
    int prime = ds::FindPrime(2 * data.nElem);
    HashResize(prime);
    return true;
}
