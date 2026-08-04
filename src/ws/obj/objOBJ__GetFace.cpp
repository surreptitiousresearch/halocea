#include "objOBJ.h"
#include "objGEOM_UNSHARED.h"
#include "objGEOM_SHARED.h"
#include "objFACE.h"
#include "../ds/ds_assert_boundary.h"
// 0x82661278  ?GetFace@objOBJ@@QAAPAUobjFACE@@H@Z
// Return the `faceNmb`-th triangle of the object's shared face list. Asserts the index is in
// range and that the faces are not stored in a compressed form.

objFACE *objOBJ::GetFace(int faceNmb)
{
    objGEOM_UNSHARED *pGeom;
    if (!IGNORE_STRONG_ASSERT &&
        (faceNmb < 0 || faceNmb >= this->nFace ||
         ((pGeom = this->pGeom) != nullptr &&
          (pGeom->pSharedGeom->stateCompr.state & 0xC0) != 0)))
        STRONG_ASSERT_DUMMY().Crash(
            "faceNmb >= 0 && faceNmb < nFace && !IsStateCompr(OBJ_ST_COMPR_FACE | OBJ_ST_COMPR_FACE_32)",
            "D:\\Projects\\code\\common\\INCL.SYS\\objects.h", 749,
            dsStrongAssertMessage);

    return &this->pGeom->pSharedGeom->faceList[faceNmb];
}
