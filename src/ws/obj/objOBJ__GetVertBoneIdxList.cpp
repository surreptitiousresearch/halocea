#include <stdint.h>
#include "objOBJ.h"
// 0x82BCC9F0  ?GetVertBoneIdxList@objOBJ@@QAAPAXH@Z
// Address of the per-vertex bone-index entry for `offset`. Stride is 4 bytes when the geometry
// is compressed (stateCompr bit 0x10 set), otherwise 8 bytes.

void *objOBJ::GetVertBoneIdxList(int offset)
{
    objGEOM_UNSHARED *geom = pGeom;
    if (!geom)
        return nullptr;

    objGEOM_SHARED *sharedGeom = geom->pSharedGeom;
    uint8_t *vertBoneIdxList = sharedGeom->vertBoneIdxList;
    int byteOffset = ((sharedGeom->stateCompr.state & 0x10) != 0) ? 4 * offset : 8 * offset;
    return &vertBoneIdxList[byteOffset];
}
