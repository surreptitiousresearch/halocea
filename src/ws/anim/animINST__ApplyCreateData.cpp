#include "animINST.h"
#include "animMNG.h"
#include "animCREATE_DATA.h"
#include "animCMD_SET_SEQ.h"
#include "animSEQ.h"
#include "animTPL.h"
#include "../m3d/m3dMATR.h"
#include "../m3d/m3d_boundary.h"
#include "../ds/dsAFFIX_READER.h"
#include "../ds/dsAFFIX_STRING.h"
#include "../ds/dsSMART_PTR.h"
#include "../ds/dsREF_COUNT.h"
#include "../ps/psSECTION.h"
#include "../scn/scnSCENE.h"
#include "../cdt/cdtCOLL_ANIM_SCN.h"
#include "../rend/rendLIGHT_SET.h"
#include "../rend/rendSR_DATA.h"

// CAVEAT: the compiler packed several logically-distinct temporaries onto one big stack scratch
// object (the decompiler surfaced it as a phantom `animINST v27`): the three GetScale out-params,
// the on-stack animCMD_SET_SEQ, one dsAFFIX_READER over this->affixes, and the two by-value
// GetLightSet return slots. They are untangled here into separately-named locals. The SET_SEQ
// build was verified byte-for-byte against the disasm (seqCmd.seqID=animSeqNmb, extID=-1,
// layerID=-1, offsetTime=FrameToTime, playBackType=cycled?CYCLE:CLAMP) and matches the vetted sibling
// animINST::PostApplyCreateData. The two dsAFFIX_READER destructors the decompiler inlined as
// buffer-refcount releases are modeled as ordinary scope exit of the reader locals.

typedef dsSMART_PTR<rendLIGHT_SET, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > LIGHT_SET_SP;

// ?ApplyCreateData@animINST@@QAAXPAVanimCREATE_DATA@@@Z  (0x825E4170) — apply a creation-parameter
// block to this instance: install the transform (scale-stripped), fire the initial animation
// sequence, copy the ps section and affixes, resolve collision/AI affix flags, set the per-instance
// scale (matrix scale times the requested scale), the name, and any shader-replacement light data.
void animINST::ApplyCreateData(animCREATE_DATA *pCD)
{
    if (!pCD)
        return;

    m3dMATR *pMatrInst = &pCD->matrInst;

    // Separate the pure scale out of the incoming transform; keep the components for the scale below.
    float matrScaleX, matrScaleY, matrScaleZ;
    pMatrInst->GetScale(&matrScaleX, &matrScaleY, &matrScaleZ);
    pMatrInst->RemoveScale();

    this->perInstLock.Lock(nullptr, 0);
    this->matrL2W.Transform(pMatrInst, 0);
    this->perInstLock.Unlock(nullptr, 0);
    this->Invalidate(2048, 1);

    // Kick off the initial animation sequence requested by the create-data (if any).
    if (this->pAnimMng && (unsigned __int16)pCD->animSeqNmb < 0x8000u)
    {
        int seqIndex = pCD->animSeqNmb;
        if (seqIndex < this->pAnimMng->GetNAnimSeq())
        {
            animSEQ *pSeq = this->pAnimMng->GetAnimSeq(seqIndex);
            if (pSeq)
            {
                animCMD_SET_SEQ cmd;
                cmd.layerID = -1;
                cmd.offsetTime = 0.0f;
                cmd.seqCmd.seqID = pCD->animSeqNmb;
                cmd.seqCmd.extID = -1;
                cmd.offsetTime = pSeq->FrameToTime(pCD->frameCur);
                cmd.playBackType = pCD->animSeqIsCycled ? ANIM_PB_CYCLE : ANIM_PB_CLAMP;
                this->pAnimMng->SendAnimCmd(cmd);
            }
        }
    }

    this->ps = pCD->ps;

    // Use the create-data affixes when present, else fall back to the template's affixes.
    dsAFFIX_STRING *pAffixes = &pCD->affixes;
    if (!pCD->affixes.str.pBuffer->strLen)
        pAffixes = &this->pTpl->affixes;
    this->SetAffixes(*pAffixes);

    // Preserve everything except the create-data-owned state bits (0x1000011).
    int stateKept = this->state & 0xFEFFFFEE;
    this->state = (pCD->state & 0x1000011) | stateKept;

    // Template affixes that disable collision -> mark the instance non-colliding and drop its
    // cached collision-animation record.
    dsAFFIX_READER tplAffixReader(this->pTpl->affixes.str); // ctor takes const dsTSTRING<char>& (affixes.str)
    if (tplAffixReader.HaveAffix("cdt_no_collision", 0)
        || tplAffixReader.HaveAffix("cdt_on_none", 0)
        || tplAffixReader.HaveAffix("cdt_off_all", 0))
    {
        scnSCENE *pScn = this->pScn;
        this->state |= 0x10u;
        if (pScn && pScn->pCollAnim)
            pScn->pCollAnim->Invalidate(this, 1);
    }

    // Instance affixes that tune AI/aim-assist behaviour.
    dsAFFIX_READER instAffixReader(this->affixes.str); // ctor takes const dsTSTRING<char>& (affixes.str)
    if (instAffixReader.HaveAffix("cdt_off_ai_look", 0))
        this->state2 |= 0x4000u;
    if (instAffixReader.HaveAffix("cdt_off_ai_move", 0))
        this->state2 |= 0x2000u;
    if (instAffixReader.HaveAffix("ai_navlink_through", 0))
        this->state2 |= 0x80000000;
    if (instAffixReader.HaveAffix("cdt_aim_assist_visibility_ignore", 0))
        this->state2 |= 0x100000u;

    // Final scale = matrix-derived scale times the create-data scale, on each axis.
    float finalScaleX = pCD->scaleX * matrScaleX;
    float finalScaleZ = pCD->scaleZ * matrScaleZ;
    float finalScaleY = pCD->scaleY * matrScaleY;
    _m3dCheckValid(finalScaleX);
    _m3dCheckValid(finalScaleY);
    _m3dCheckValid(finalScaleZ);

    this->perInstLock.Lock(nullptr, 0);
    this->scale.x = finalScaleX;
    this->scale.y = finalScaleY;
    this->scale.z = finalScaleZ;
    this->perInstLock.Unlock(nullptr, 0);
    this->Invalidate(2048, 1);
    this->Invalidate(2048, 1); // (emitted twice by the compiler)

    if (pCD->name[0])
        this->SetName(pCD->name);

    // Optional shader-replacement data: create/attach a light set, then push the SR data into it.
    if (pCD->srData)
    {
        LIGHT_SET_SP emptySet;
        emptySet.pointee = nullptr;
        this->AddLightSet(emptySet);

        LIGHT_SET_SP probe = this->GetLightSet();
        bool hasLightSet = probe.pointee != nullptr;
        probe.DeletePointee();

        if (hasLightSet)
        {
            LIGHT_SET_SP lightSet = this->GetLightSet();
            pCD->ApplySRData(&lightSet.pointee->srData);
            lightSet.DeletePointee();
        }
    }
}
