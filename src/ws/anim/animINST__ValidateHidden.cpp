#include "animINST.h"
#include "animTPL.h"
#include "tplSKIN.h"
#include "objOBJ_boundary.h"
#include "anim_validate_boundary.h"
#include "../ds/ds_assert_boundary.h"

// ?ValidateHidden@animINST@@QAAXXZ  (0x82B107F0) — recompute skin-compound origins for the hidden
// skin objects in the render list, then, for each affected skin bone, clear the pending flag on its
// compound objects and drop stale modifiers.
void animINST::ValidateHidden()
{
    tplSKIN *pSkin = this->pTpl->pSkin;
    if (!pSkin)
        return;

    int nUpdated = 0;
    for (int i = 0; i < this->nObjRend; ++i)
    {
        objOBJ *pObj = this->ppObjListRend[i];
        if (pObj && (pObj->state & 8) != 0)
        {
            pObj->stateProc |= 8u;
            this->UpdateSkinCompOrigin(pObj);
            ++nUpdated;
        }
    }

    if (!nUpdated || pSkin->nBones <= 0)
        return;

    for (int b = 0; b < pSkin->nBones; ++b)
    {
        objOBJ *pBoneObj = this->ppObjList[b];
        if (!pBoneObj || (pBoneObj->state & 0x40000) == 0 || (pBoneObj->stateProc & 0x41100DF) != 0)
            continue;

        // clear the skin-compound pending bit on every compound object of this bone's split table
        objSPLIT *pSplit = pBoneObj->pGeom->pSharedGeom->pSplit;
        for (int e = 0; e < pSplit->nEntry; ++e)
        {
            objSPLIT_ENTRY *entry = pSplit->GetEntry(e);
            objOBJ *pCompObj = this->GetObj(entry->skinCompoundId);
            pCompObj->stateProc &= ~8u;
        }

        // second pass: verify each compound is a skin-compound and drop modifiers if any carries them
        objSPLIT *pSplitRe = pBoneObj->pGeom->pSharedGeom->pSplit;
        for (int e = 0; e < pSplitRe->nEntry; ++e)
        {
            objSPLIT_ENTRY *entry = pSplitRe->GetEntry(e);
            objOBJ *pCompObj = this->GetObj(entry->skinCompoundId);
            if (!IGNORE_STRONG_ASSERT && (pCompObj->state & 8) == 0)
                STRONG_ASSERT_DUMMY().Crash(
                    "pObjSkin->IsSkinCompound()",
                    "D:\\Projects\\code\\common\\src.sys\\objects\\obj_skin.cpp",
                    1348,
                    dsStrongAssertMessage);
            if (pCompObj->pGeom->nModifiers)
            {
                dsTSTRING_BUF_HEADER<char> *pAffixBuffer = pBoneObj->affixes.str.pBuffer;
                const char *name = this->GetName();
                _apLog("~Skin~ Removed modifiers. Inst %s. Affixes %s", name, pAffixBuffer->str);
                pBoneObj->RemoveModifiers();
            }
            pSplitRe = pBoneObj->pGeom->pSharedGeom->pSplit;
        }
    }
}
