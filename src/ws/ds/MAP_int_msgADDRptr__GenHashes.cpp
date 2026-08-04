#include "MAP.h"
#include "../m3d/m3dRND_GEN.h"
struct msgADDR; // fwd (pointer element only)

extern m3dRND_GEN m3dRndGen; // boundary — process-wide PRNG used to seed hash-table salts

// ds::MAP<int,msgADDR*,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::GenHashes @ 0x82556670
// Reseed the two hash salts: `a` is a prime near a fresh random value (biases the modulus mixing
// ds::HASH::hash<K> performs), `b` is a plain random salt.
template<>
void ds::MAP<int, msgADDR *, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::GenHashes()
{
    a = ds::FindPrime(m3dRndGen.RndInt() + 10);
    b = m3dRndGen.RndInt();
}
