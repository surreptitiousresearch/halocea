#include "../../headers/ws/ai/aiWATCHER_UpdateHeights_boundary.h"
#include "../../headers/ws/cdt/cdtBONE.h"
#include "../../headers/ws/m3d/m3d_boundary.h"

// `anonymous namespace'::CalcYHeightFromMatrToBone @ 0x8328D658
// ?CalcYHeightFromMatrToBone@?A0x........@@YAMPAVanimINST@@ABUm3dV@@1PBVcdtBONE@@@Z
// Height, measured along `dirUp`, from `posStart` up to `bone`'s world-space centre within `inst`
// (i.e. |bone - posStart| projected onto dirUp, via cos(angle)*length), floored at 0.2.
// Binary places this in an anonymous namespace; kept a shared free function across the split TUs.
float CalcYHeightFromMatrToBone(animINST *inst, const m3dV &posStart, const m3dV &dirUp,
                                const cdtBONE *bone)
{
    m3dV boneCenter = bone->GetCenter(inst);
    _m3dCheckValid(&boneCenter);
    _m3dCheckValid(&posStart);

    m3dV delta;
    delta.x = boneCenter.x - posStart.x;
    delta.y = boneCenter.y - posStart.y;
    delta.z = boneCenter.z - posStart.z;

    _m3dCheckValid(&dirUp);
    _m3dCheckValid(&delta);

    float height = m3dAngleVector_COS(&dirUp, &delta) * m3dLengthVector(&delta);
    if (height < 0.2f)
        height = 0.2f;
    return height;
}
