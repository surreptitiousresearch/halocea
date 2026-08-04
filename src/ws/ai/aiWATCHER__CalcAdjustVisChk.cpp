#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/m3d/m3d_boundary.h"      // m3dVZero, m3dV::operator+=

// aiWATCHER::CalcAdjustVisChk @ 0x8328F408
// ?CalcAdjustVisChk@aiWATCHER@@QBA?AUm3dV@@PBVaiBRAIN@@@Z
//
// Per-viewer visibility-check position adjustment while hidden: the sum of the vertical and
// horizontal view-cover adjustments for `observer`. (PPC struct-return: the decompiler aliases the
// hidden return buffer as `this`; the first three float writes are the zero-init of the result.)
m3dV aiWATCHER::CalcAdjustVisChk(const aiBRAIN *observer) const
{
    m3dV result = m3dVZero;
    result += this->CalcVerticalVCAdjustHO(observer);
    result += this->CalcHorizontalVCAdjustHO(observer);
    return result;
}
