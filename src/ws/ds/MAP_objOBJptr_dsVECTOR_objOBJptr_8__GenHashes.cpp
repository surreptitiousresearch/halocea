#include "MAP.h"
struct objOBJ; // boundary — map key type, used pointer-only
#include "../m3d/m3dRND_GEN.h"

extern m3dRND_GEN m3dRndGen; // boundary — process-wide PRNG used to seed hash-table salts

// ds::MAP<objOBJ*,dsVECTOR<objOBJ*,8>,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::GenHashes @ 0x82681220
// Reseed the two hash salts: `a` is a prime near a fresh random value, `b` is a plain random salt.
template<>
void ds::MAP<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::GenHashes()
{
    a = ds::FindPrime(m3dRndGen.RndInt() + 10);
    b = m3dRndGen.RndInt();
}
