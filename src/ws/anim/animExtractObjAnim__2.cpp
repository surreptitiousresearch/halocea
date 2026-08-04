// animExtractObjAnim(objOBJ*, animSEQ*, float, animTRS*) @0x82B2D2C8
// animTRS-producing overload: sample the full decomposed transform of object `pObj` in sequence
// `pSeq` at `frame` into *trs. Resolves the pairing through the object's animation manager; on
// success delegates to animExtractAnimTrs (with the object's additive-blend state bit 0x20000),
// otherwise fills *trs with an identity transform (zero translate, unit scale, identity rotation)
// and sets the "defaulted" type flags 0x4E. Returns true when a pairing existed.
//
// DEVIATION: the decompiler split the single animTRS* out-param into trs + a5 and mis-typed the
// additive flag as a pointer; the disasm (0x82B2D374: rlwinm r5,r11,0,14,14 = state & 0x20000;
// r6 = trs) confirms one animTRS* out-param and an int additive flag.
#include "../../headers/ws/obj/objOBJ.h"
#include "../../headers/animINST.h"
#include "../../headers/ws/anim/animMNG.h"
#include "../../headers/ws/anim/animOBJ_ANIM.h"
#include "../../headers/ws/anim/animTRS.h"

struct animSEQ;

extern const m3dV m3dVZero;  // boundary — the zero vector global

bool animExtractObjAnim(objOBJ *pObj, animSEQ *pSeq, float frame, animTRS *trs)
{
    trs->type.state = 0;

    animINST *pInst = pObj->pInst;
    const animOBJ_ANIM *objAnim = pInst->pAnimMng->__vftable->GetObjAnim(pInst->pAnimMng, pObj, pSeq);
    if (!objAnim)
    {
        trs->trans = m3dVZero;
        trs->rot.Identity();
        trs->scale.z = 1.0f;
        trs->scale.y = 1.0f;
        trs->scale.x = 1.0f;
        trs->type.state |= 0x4E;
        return false;
    }

    animExtractAnimTrs(objAnim, frame, pInst->state & 0x20000, trs);
    return true;
}
