#include "../../headers/ws/cdt/cdtSKEL.h"
#include "../../headers/ws/anim/animINST.h"
#include "../../headers/ws/anim/animMNG.h"

// 0x82B0B5D0 — applies the pose with the ApplyAnim "recursively apply, exclusive lock" flag (4).
void cdtSKEL::CalcAnimExcl(animINST *pInst)
{
    pInst->pAnimMng->ApplyAnim(4);
}
