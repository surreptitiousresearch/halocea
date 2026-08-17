/* ?_AllocFaceList@objOBJ@@QAAHH@Z @0x82A803A0 */
#include <string.h>
#include "objOBJ.h"
#include "objGEOM_UNSHARED.h"
#include "objGEOM_SHARED.h"
#include "objFACE.h"
#include "obj_boundary.h"
// 0x82A803A0  ?_AllocFaceList@objOBJ@@QAAHH@Z
// (Re)allocate the face list to `nFace` faces. Face stride is 6 bytes, or 12 when the shared
// geometry flags faces compressed (stateCompr bit 0x80). Routes through the pluggable dataAlloc hook
// when set, else dlRealloc. Newly-grown faces are zeroed. Returns 1 on success, 0 on failure.

int objOBJ::_AllocFaceList(int nFace)
{
    if (!this->pGeom)
        return 0;

    // STRONG_ASSERT: !stateShare.Is(OBJ_ST_SHARED_FACE_LIST)  (Obj_alloc.cpp:592)
    objGEOM_UNSHARED *pGeom = this->pGeom;
    bool compressed = pGeom && (pGeom->pSharedGeom->stateCompr.state & 0x80) != 0;
    objGEOM_SHARED *pSharedGeom = pGeom->pSharedGeom;
    int stride = compressed ? 12 : 6;

    objFACE *newFaces = (objFACE *)(objOBJ::dataAlloc
        ? objOBJ::dataAlloc(this, 37, stride * nFace, pSharedGeom->faceList)
        : dlRealloc(pSharedGeom->faceList, stride * nFace,
                    "D:\\Projects\\code\\common\\src.sys\\objects\\Obj_alloc.cpp", 0x50D));
    if (!newFaces && nFace > 0)
        return 0;

    _UnshareGeom();
    this->pGeom->pSharedGeom->faceList = newFaces;

    int oldFace = this->nFace;
    this->nFace = nFace;
    if (nFace > oldFace) {
        objFACE *face = GetFace(oldFace);
        memset(face, 0, (nFace - oldFace) * stride);
    }
    return 1;
}
