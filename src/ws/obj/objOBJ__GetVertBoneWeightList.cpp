#include "objOBJ.h"
// 0x82BCC998  ?GetVertBoneWeightList@objOBJ@@QAAPAXH@Z
// Address of the per-vertex bone-weight entry for `offset`. When the geometry is compressed
// (stateCompr bit 0x20 set) weights are indexed 1-per-vertex, otherwise 4-per-vertex.

void *objOBJ::GetVertBoneWeightList(int offset)
{
    objGEOM_UNSHARED *geom = pGeom;
    if (geom)
    {
        objGEOM_SHARED *sharedGeom = geom->pSharedGeom;
        if ((sharedGeom->stateCompr.state & 0x20) != 0)
            return &sharedGeom->vertBoneWeightList[offset];
    }
    return &pGeom->pSharedGeom->vertBoneWeightList[4 * offset];
}
