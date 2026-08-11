#include "../headers/hcex/HALO_MNG_DESC.h"
#include "../headers/ws/os/os_boundary.h" // dlFree

// @0x823E5500 — HALO_MNG_DESC has no fields of its own. The disassembly shows this address
// releasing six inherited iaDESC/sslDESC sub-objects in sequence: spSndData, smlDesc's
// name-index vector storage, smlProg, psCustomSection, propContainerDesc, then sslClass -- this
// is the compiler's inlined *complete* destructor epilogue for the whole base chain (iaDESC has
// no separately-addressed ~iaDESC of its own here).
//
// DEVIATION: three of those six (spSndData, smlProg, propContainerDesc -- all dsSMART_PTR, which
// per dsSMART_PTR.h has NO destructor of its own) and the raw vector-storage free are NOT
// automatic in C++ and must be called out explicitly. The other two (psCustomSection: psSECTION,
// sslClass: sslCLASS_REF) DO have real destructors (see psSECTION.h / sslCLASS_REF.h) -- writing
// explicit `.~Type()` calls for those here, on top of the compiler's own automatic per-member
// destruction at the end of ~HALO_MNG_DESC(), would destroy them twice. Reconstructed below with
// only the non-automatic releases explicit; psCustomSection/sslClass are left to be destroyed
// exactly once, automatically, matching the binary's actual (single-destruction) behavior.
HALO_MNG_DESC::~HALO_MNG_DESC()
{
    spSndData.DeletePointee();
    dlFree(smlDesc.nameIdx.pData);
    smlProg.DeletePointee();
    propContainerDesc.DeletePointee();
}
