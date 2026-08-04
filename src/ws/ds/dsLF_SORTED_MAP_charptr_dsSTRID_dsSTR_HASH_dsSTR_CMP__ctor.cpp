#include "dsLF_SORTED_MAP.h"
#include "ds_boundary.h"
#include "../m3d/m3dRND_GEN.h"
#include "dsSTRID.h"
#include "dsSTR_HASH.h"
struct dsSTR_CMP; // boundary functor (opaque; template arg only)

extern m3dRND_GEN m3dRndGen; // boundary — process-wide PRNG used to seed hash-table salts

// dsLF_SORTED_MAP<char const*,dsSTRID,dsSTR_HASH,dsSTR_CMP>::dsLF_SORTED_MAP @ 0x825291F0
// Construct with the given allocation call-site cookie: the bucket-array dsVECTOR's own
// dsVECTOR(apCL) ctor is inlined at this address (null storage / zero count, __cl = cl), then
// this map's own `cl` copy is recorded, and the lock-free hash salt pair is seeded the same way
// every sibling ds hash container does (a = FindPrime(RndInt()+10), b = RndInt()).
template<>
dsLF_SORTED_MAP<const char *, dsSTRID, dsSTR_HASH, dsSTR_CMP>::dsLF_SORTED_MAP(apCL cl)
    : hashTable(cl)
{
    this->cl = cl;

    this->a = ds::FindPrime(m3dRndGen.RndInt() + 10);
    this->b = m3dRndGen.RndInt();
}
