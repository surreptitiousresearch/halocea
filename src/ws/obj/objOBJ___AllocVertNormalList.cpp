#include "objOBJ.h"
#include "objGEOM_UNSHARED.h"
#include "objGEOM_SHARED.h"
// 0x82A7F420  ?_AllocVertNormalList@objOBJ@@QAAHH@Z
// (Re)allocate the vertex-normal array: 12 bytes/vertex (m3dV) normally, 4 bytes when the
// geometry stores compressed normals (stateCompr bit 0x4). Returns 1 on success (or when
// nVert==0), 0 on allocation failure.

int objOBJ::_AllocVertNormalList(int nVert)
{
    objGEOM_UNSHARED *pGeom = this->pGeom;
    if (!pGeom)
        return 0;

    int elemSize = 12; // m3dV
    if (pGeom->pSharedGeom->stateCompr.state & 4)
        elemSize = 4;  // compressed normal

    void *normals = AllocObjData(3, elemSize * nVert, pGeom->pSharedGeom->normalVertList);
    if (!normals && nVert > 0)
        return 0;

    SetVertNormalList((m3dV *)normals);
    return 1;
}
