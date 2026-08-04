// animExtractObjAnim(objOBJ*, animSEQ*, float, m3dQUAT*) @0x82B2BD60
// Sample the rotation of object `pObj` in animation sequence `pSeq` at `frame` into *outRot.
// Resolves the object/sequence pairing through the object's animation manager; on success reads
// the rotation channel (using the object's additive-blend state bit 0x20000), otherwise fills
// *outRot with the identity rotation. Returns true when a pairing existed.
//
// DEVIATION: the decompiler modelled the single output-quaternion parameter as two args
// (tr + a5) and mis-typed the additive flag as a pointer; the disasm (0x82B2BDC4:
// rlwinm r5,r11,0,14,14 = state & 0x20000; r6 = outRot) shows one m3dQUAT* out-param and an int
// additive flag passed to GetRotation.
#include "../../headers/ws/obj/objOBJ.h"
#include "../../headers/animINST.h"
#include "../../headers/ws/anim/animMNG.h"
#include "../../headers/ws/anim/animOBJ_ANIM.h"
#include "../../headers/ws/m3d/m3dQUAT.h"

struct animSEQ;

bool animExtractObjAnim(objOBJ *pObj, animSEQ *pSeq, float frame, m3dQUAT *outRot)
{
    animINST *pInst = pObj->pInst;
    const animOBJ_ANIM *objAnim = pInst->pAnimMng->__vftable->GetObjAnim(pInst->pAnimMng, pObj, pSeq);
    if (!objAnim)
    {
        outRot->Identity();
        return false;
    }

    objAnim->GetRotation(frame, pInst->state & 0x20000, outRot);
    return true;
}
