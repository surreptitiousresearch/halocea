#include "animINST.h"
#include "animTPL.h"
#include "tplSKIN.h"
#include "animMNG.h"
#include "animSEQ.h"
#include "anim_validate_boundary.h"
#include <math.h>
#include <string.h>

// ?ValidateOBB@animINST@@AAAXXZ  (0x825E3220, private) — recompute the instance's oriented bounding
// box (and enclosing AABB) from the best available source: the collision skeleton, the physics
// ragdoll, the current animation sequence, or the template default box. Applies per-axis scale and
// the box expansion margin, then refreshes the visual-distance factor.
void animINST::ValidateOBB()
{
    if ((this->state2 & 0x80000) == 0)
    {
        m3dMATR orthoL2W;
        this->CalcOrthoL2W(&orthoL2W);

        animTPL *pTpl = this->pTpl;
        // template default box (the decompiler open-coded this 6-dword copy from pTpl->pObj+4)
        m3dBOX box = pTpl->bbox;
        bool needTransform = true;

        if ((pTpl->state & 0x400000) == 0)
        {
            cdtSKEL *pSkel = this->pSkel;
            if (!pSkel && pTpl && pTpl->pSkin && (this->state & 0x4000000) != 0)
                pSkel = pTpl->pSkin->pCdtSkel;

            if (pSkel)
            {
                pSkel->GetEnclBox(this, &box);
                needTransform = false;
            }
            else
            {
                physRAGDOLL_BASE *pPhysChar = this->pPhysChar;
                if (pPhysChar)
                {
                    pPhysChar->CalcBBox_2(pPhysChar, &box);
                    needTransform = false;
                }
                else
                {
                    animMNG *pAnimMng = this->pAnimMng;
                    if (pAnimMng)
                    {
                        anim_blend::SEQ_ID_FULL seqId = this->pAnimMng->GetCurAnimSeq(0);
                        if (seqId.seqID != -1)
                        {
                            animSEQ *pSeq = pAnimMng->GetAnimSeq_2(&seqId);
                            if (pSeq && (pSeq->stateSeq & 0x100) != 0)
                                box = pSeq->bbox; // decompiler open-coded this 6-dword copy
                        }
                    }
                }
            }
        }

        float scaleX = this->scale.x;
        float scaleY = this->scale.y;
        float scaleZ = this->scale.z;
        if (fabsf(scaleX) < 0.001f)
            scaleX = 0.001f;
        if (fabsf(scaleY) < 0.001f)
            scaleY = 0.001f;
        if (fabsf(scaleZ) < 0.001f)
            scaleZ = 0.001f;

        if (needTransform)
        {
            box.bll.x = box.bll.x * scaleX;
            box.bll.y = box.bll.y * scaleY;
            box.bll.z = box.bll.z * scaleZ;
            box.fur.x = box.fur.x * scaleX;
            box.fur.y = box.fur.y * scaleY;
            box.fur.z = box.fur.z * scaleZ;
        }

        if (scaleX < 0.0f || scaleY < 0.0f || scaleZ < 0.0f)
            box.MakePointPoint(&box.bll, &box.fur);

        m3dOBB obb;
        if (needTransform)
            box.Transform(&orthoL2W, &obb);
        else
            box.ConvToOBB(&obb);

        this->perInstLock.Lock(nullptr, 0);
        memcpy(&this->obb, &obb, sizeof(this->obb));
        this->obb.Expand(this->obbExpansion);
        this->obb.CalcEnclBox(&this->bbox);
        this->perInstLock.Unlock(nullptr, 0);
    }

    this->CalcVisDistFactor();
}
