/* ?SetVertBoneIdxList@objOBJ@@QAAXPAX@Z @0x82BCB3E8 */
#include <stdint.h>
#include "objOBJ.h"
// 0x82BCB3E8  ?SetVertBoneIdxList@objOBJ@@QAAXPAX@Z
// Store the per-vertex bone-index list into the shared geometry and set/clear FVF bit 0x800
// to advertise its presence.
// NOTE: the decompiler's HIDWORD(fvf) packing is a 32-bit-split artifact; the disassembly
// (ld/std @0x8, ori 0x800 / andi ~0x800) operates on the whole 64-bit fvf. Corrected here.

void objOBJ::SetVertBoneIdxList(uint8_t *idxList)
{
    _UnshareGeom();
    objGEOM_SHARED *sharedGeom = pGeom->pSharedGeom;
    sharedGeom->vertBoneIdxList = idxList;
    if (idxList)
        sharedGeom->fvf |= 0x800;
    else
        sharedGeom->fvf &= ~(uint64_t)0x800;
}
