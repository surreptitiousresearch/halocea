// hcexHALO_MODEL__OnLoadInst @0x823D6F38
#include "../headers/hcex/hcexHALO_MODEL.h"
#include "../headers/animINST.h"

// 0x823D6F38
// Run the base region-substitution hook, then apply the fixed HCEX->Halo coordinate correction:
// scale by the inch->metre factor (0.03048) and yaw -90 degrees.
void hcexHALO_MODEL::OnLoadInst(animINST *pInst)
{
    hcexHALO_OBJECT::OnLoadInst(pInst);
    // DEVIATION: operOrder is 1, not 0. Both call sites set it with an explicit immediate --
    // `li r7, 1` @0x823D6F58 before Scale, `li r5, 1` @0x823D6F70 before RotateY -- so the
    // previous note here ("leftover reg in decomp, source default 0") was refuted by the
    // instruction it was written about. Found by the linkdup drain: the duplicate copy of this
    // TU had the right value and the wrong callee, this one the right callee and the wrong value,
    // so deleting either without reading both would have kept a defect.
    pInst->Scale(0.030479999f, 0.030479999f, 0.030479999f, 1);
    pInst->RotateY(-90.0f, 1);
}
