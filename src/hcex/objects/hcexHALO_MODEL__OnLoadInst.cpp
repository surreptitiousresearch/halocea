/* hcexHALO_MODEL::OnLoadInst @0x823D6F4C (overrides hcexHALO_OBJECT::OnLoadInst) — run the base
 * region-substitution hook, then apply the fixed HCEX model->Halo unit conversion: uniform scale
 * by 0.03048 (verified operOrder=1 in the disassembly) and a -90 degree yaw (RotateY, operOrder=1),
 * both appended to the instance's existing transform. */

#include "../../headers/hcex/hcexHALO_MODEL.h"
#include "../../headers/ws/anim/animINST.h"

void hcexHALO_MODEL::OnLoadInst(animINST *pInst)
{
    hcexHALO_OBJECT::OnLoadInst(pInst);
    pInst->Scale(0.03048f, 1);
    pInst->RotateY(-90.0f, 1);
}
