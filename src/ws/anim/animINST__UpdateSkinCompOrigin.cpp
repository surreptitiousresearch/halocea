#include "animINST.h"
#include "objOBJ_boundary.h"
#include "anim_validate_boundary.h"

// ?UpdateSkinCompOrigin@animINST@@QAAXPAVobjOBJ@@@Z  (0x82B0F4F0) — recompute a skin-compound
// object's origin as the average world position of the skin bones that feed it, and write that
// average into its local-to-world translation.
// CAVEAT: the decompiler packed pSharedGeom and the bone-index span into HIDWORD/LODWORD halves of
// a shared 64-bit register slot; untangled here into named locals (verified against the objGEOM_*
// layout in the DB).
void animINST::UpdateSkinCompOrigin(objOBJ *pSkinCompObj)
{
    int nAccum = 0;
    m3dV sumPos = m3dVZero;

    objGEOM_SHARED *pSharedGeom = pSkinCompObj->pGeom->pSharedGeom;
    unsigned int first = pSharedGeom->vertBoneIdxFirst;

    for (int i = 0; i < (int)pSharedGeom->vertBoneIdxCount; ++i)
    {
        objOBJ *pBoneObj = this->ppObjList[first + i];
        if (pBoneObj && (pBoneObj->state & 0x20000) != 0)
        {
            m3dV bonePos;
            bonePos.x = pBoneObj->matrLT.elements[12];
            bonePos.y = pBoneObj->matrLT.elements[13];
            bonePos.z = pBoneObj->matrLT.elements[14];
            _m3dCheckValid(&bonePos);
            sumPos += bonePos;
            ++nAccum;
        }
    }

    if (nAccum > 0)
    {
        float invCount = 1.0f / (float)nAccum;
        _m3dCheckValid((float)nAccum);
        sumPos.x = sumPos.x * invCount;
        sumPos.y = sumPos.y * invCount;
        sumPos.z = sumPos.z * invCount;
        _m3dCheckValid(&sumPos);
        pSkinCompObj->matrLT.elements[12] = sumPos.x;
        pSkinCompObj->matrLT.elements[13] = sumPos.y;
        pSkinCompObj->matrLT.elements[14] = sumPos.z;
    }
}
