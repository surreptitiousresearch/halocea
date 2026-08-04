#include "../headers/hcex/hcexHALO_MODEL.h"
#include "../headers/animINST.h"

// 0x823D6F38
// Run the base region-substitution hook, then apply the fixed HCEX->Halo coordinate correction:
// scale by the inch->metre factor (0.03048) and yaw -90 degrees.
void hcexHALO_MODEL::OnLoadInst(animINST *pInst)
{
    hcexHALO_OBJECT::OnLoadInst(pInst);
    pInst->Scale(0.030479999f, 0.030479999f, 0.030479999f, 0); // operOrder: leftover reg in decomp, source default 0
    pInst->RotateY(-90.0f, 0);                                  // operOrder: leftover reg in decomp, source default 0
}
