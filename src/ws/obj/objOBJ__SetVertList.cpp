#include "objOBJ.h"
#include "objGEOM_UNSHARED.h"
#include "objGEOM_SHARED.h"
// 0x82BCC7E0  ?SetVertList@objOBJ@@QAAXPAX@Z
// Install the vertex-position array (unsharing the geometry first) and update the FVF present /
// compression bits accordingly: position present = 0x1, compression flags 0x8 (stateCompr&0x1)
// and 0x2 (stateCompr&0x2).
//
// CAVEAT: the decompiler packed the pSharedGeom pointer and fvf into overlapping 64-bit slots;
// untangled against the disassembly.

void objOBJ::SetVertList(m3dV *list)
{
    _UnshareGeom();

    objGEOM_SHARED *pSharedGeom = this->pGeom->pSharedGeom;
    pSharedGeom->vertList = list;
    unsigned __int64 fvf = pSharedGeom->fvf;

    if (list) {
        pSharedGeom->fvf = fvf | 1;                   // FVF: position present
        if (pSharedGeom->stateCompr.state & 1) {      // compressed position
            pSharedGeom->fvf |= 8;
            if (pSharedGeom->stateCompr.state & 2)    // wider compressed form
                pSharedGeom->fvf |= 2;
        }
    } else {
        pSharedGeom->fvf = fvf & ~1ull;               // clear position present
        pSharedGeom->fvf &= ~6ull;                    // clear 0x2 | 0x4
        pSharedGeom->fvf &= ~8ull;                    // clear 0x8
    }
}
