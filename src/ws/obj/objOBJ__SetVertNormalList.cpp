#include "objOBJ.h"
#include "objGEOM_UNSHARED.h"
#include "objGEOM_SHARED.h"
// 0x82BCCB40  ?SetVertNormalList@objOBJ@@QAAXPAX@Z
// Install the vertex-normal array (unsharing the geometry first) and update the FVF present /
// compression bits accordingly.
//
// CAVEAT: the decompiler packed the pSharedGeom pointer and the fvf into overlapping 64-bit
// slots; untangled against the disasm (0x82BCCB60-0x82BCCC40). Note the asymmetry preserved
// from the binary: setting a list sets 0x1000 and (if stateCompr&0x4) 0x2000; clearing a list
// clears 0x1000 and 0x2000 but then, if stateCompr&0x8, SETS 0x8000.

void objOBJ::SetVertNormalList(m3dV *list)
{
    _UnshareGeom();

    objGEOM_SHARED *pSharedGeom = this->pGeom->pSharedGeom;
    pSharedGeom->normalVertList = list;
    unsigned __int64 fvf = pSharedGeom->fvf;

    if (list) {
        pSharedGeom->fvf = fvf | 0x1000;              // FVF: normal present
        if (pSharedGeom->stateCompr.state & 4)        // compressed normal
            pSharedGeom->fvf |= 0x2000;
    } else {
        pSharedGeom->fvf = fvf & ~0x1000ull;          // clear normal present
        pSharedGeom->fvf &= ~0x2000ull;               // clear compressed-normal
        if (pSharedGeom->stateCompr.state & 8)
            pSharedGeom->fvf |= 0x8000;               // (see CAVEAT)
    }
}
