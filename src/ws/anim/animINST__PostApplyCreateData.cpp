/* ?PostApplyCreateData@animINST@@QAAXPAVanimCREATE_DATA@@@Z @0x825E1148 */
#include <stdint.h>
#include "animINST.h"
#include "animMNG.h"
#include "animCREATE_DATA.h"
#include "animCMD_SET_SEQ.h"
#include "animSEQ.h"

// ?PostApplyCreateData@animINST@@QAAXPAVanimCREATE_DATA@@@Z  (0x825E1148) — after the base object
// has consumed the creation data, kick off the initial animation sequence requested by `pCD`
// (if any) by dispatching a SET_SEQ command to the animation manager.
void animINST::PostApplyCreateData(animCREATE_DATA *pCD)
{
    if (!pCD || !this->pAnimMng)
        return;

    unsigned int seqNmb = (uint16_t)pCD->animSeqNmb;
    if (seqNmb >= 0x8000)              // 0xFFFF sentinel = "no sequence"
        return;

    int seqIndex = (int16_t)seqNmb;
    if (seqIndex >= this->pAnimMng->GetNAnimSeq())
        return;

    animSEQ *pSeq = this->pAnimMng->GetAnimSeq(seqIndex);
    if (!pSeq)
        return;

    animCMD_SET_SEQ cmd;
    cmd.layerID = -1;
    cmd.seqCmd.seqID = pCD->animSeqNmb;
    cmd.seqCmd.extID = -1;
    cmd.offsetTime = 0.0f;
    cmd.playBackType = ANIM_PB_DEFAULT;
    // Deviation: the decompiler passed the sequence NUMBER as the FrameToTime `this`; the disasm
    // (r3 still holds the GetAnimSeq result) shows it is the resolved sequence pointer.
    cmd.offsetTime = pSeq->FrameToTime(pCD->frameCur);
    cmd.playBackType = pCD->animSeqIsCycled ? ANIM_PB_CYCLE : ANIM_PB_CLAMP;

    this->pAnimMng->SendAnimCmd(cmd);
}
