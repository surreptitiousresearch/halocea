/* ?CalcVisDistFactor@animINST@@AAAXXZ @0x825E1828 */
#include "animINST.h"
#include "anim_boundary.h"
#include "../m3d/m3d_boundary.h"

// ?CalcVisDistFactor@animINST@@AAAXXZ  (0x825E1828) — private (AAA).
// Derive the visibility-distance factor from the oriented bounding-box size: cache size^3 * 200,
// validate it, then store its square root.
void animINST::CalcVisDistFactor()
{
    float obbSize = this->obb.GetSize();
    float visDistScaled = obbSize * obbSize * obbSize * 200.0f;
    this->fVisDistFactor = visDistScaled;
    _m3dCheckValid(visDistScaled);
    this->fVisDistFactor = __fsqrts(visDistScaled);
}
