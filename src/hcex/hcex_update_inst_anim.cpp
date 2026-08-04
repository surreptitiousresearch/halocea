// hcex_update_inst_anim @ 0x823D3458
//
// Drive one animated bridge instance for a single frame: build a "play sequence" command that seeks
// `pSeq` to a frame remapped from (frameA,frameB) into the sequence's [startFrame,endFrame] span
// (via m3dClamp), send it to the instance's animation manager, then advance the manager by `dt`.
//
// Signature recovered from the mangled name (?hcex_update_inst_anim@@YAXPAVanimINST@@MABUSEQ_ID_FULL@
// anim_blend@@PAVanimSEQ@@HH@Z): void(animINST*, float, const anim_blend::SEQ_ID_FULL&, animSEQ*,
// int, int). The PPC decompiler mis-rendered this as taking two animSEQ* and two __int64s and a
// double; the disassembly (823D3458) confirms r3=pInst, f1=dt, r5=seqID, r6=pSeq, r7/r8 the two int
// frame args, converted to float via fcfid. frameA/frameB are named for their observed role (the
// two clamp inputs); their precise game-side meaning is not further resolved here.

#include "../headers/hcex/hcex_breakable_surf.h"   // pulls animINST.h (pAnimMng, UpdateAnimation)
#include "../headers/ws/anim/animSEQ.h"            // startFrame/endFrame, FrameToTime
#include "../headers/ws/anim/animMNG.h"            // SendAnimCmd
#include "../headers/ws/anim/animCMD_PLAY_SEQ.h"   // animCMD_PLAY_SEQ + its vftable
#include "../headers/ws/anim/animMNG_UPDATE_INFO.h"
#include "../headers/ws/anim/animUPDATE_STATUS.h"
#include "../headers/ws/anim/anim_blend_SEQ_INFO.h" // anim_blend::SEQ_ID_FULL

extern "C" float m3dClamp(float, float, float, float, float); // boundary — m3d

void hcex_update_inst_anim(animINST *pInst, float dt, const anim_blend::SEQ_ID_FULL &seqID,
                           animSEQ *pSeq, int frameA, int frameB)
{
    float span   = pSeq->endFrame - pSeq->startFrame;
    float mapped = m3dClamp(0.0f, span, 0.0f, (float)frameA, (float)frameB);
    float playFrame = pSeq->startFrame + mapped;

    animCMD_PLAY_SEQ cmd;
    cmd.__vftable    = (animCMD_BASE_vtbl *)&animCMD_PLAY_SEQ__vftable;
    cmd.layerID      = -1;
    cmd.seqCmd       = seqID;                    // copy the 8-byte SEQ_ID_FULL (disasm: ld/std r7)
    cmd.typeTran     = 0;
    cmd.playBackType = ANIM_PB_MANUAL;   /* DB ANIM_PLAYBACK_TYPE::ANIM_PB_MANUAL = 3 */
    cmd.offsetTime   = pSeq->FrameToTime(playFrame);
    cmd.blendTime    = -1.0f;
    cmd.playRate     = 1.0f;
    pInst->pAnimMng->SendAnimCmd(cmd);

    animMNG_UPDATE_INFO updateInfo;
    updateInfo.playRate   = 1.0f;
    updateInfo.dt         = dt;
    updateInfo.state.state = 0;

    animUPDATE_STATUS status;
    status.stateOut.state = 0;
    pInst->UpdateAnimation(updateInfo, status);
}
