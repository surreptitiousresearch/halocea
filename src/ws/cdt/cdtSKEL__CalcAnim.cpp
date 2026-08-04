#include "../../headers/ws/cdt/cdtSKEL.h"
#include "../../headers/ws/anim/animINST.h"
#include "../../headers/ws/anim/animMNG.h"

// 0x82B0B5E8 — applies the pose with the ApplyAnim "recursively apply" flag (2).
void cdtSKEL::CalcAnim(animINST *pInst)
{
    pInst->pAnimMng->ApplyAnim(2);
}
