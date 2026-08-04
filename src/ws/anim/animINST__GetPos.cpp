#include "animINST.h"
#include "../m3d/m3d_boundary.h"

// ?GetPos@animINST@@QAAXPAUm3dV@@@Z  (0x825E14D8) — copy out the instance's world-space position
// (the translation row of the local-to-world transform), under the per-instance lock.
void animINST::GetPos(m3dV *pos)
{
    this->perInstLock.Lock(nullptr, 0);
    *pos = this->matrL2W.v[3];   // row 3 = translation; m4dV sliced to its m3dV part
    _m3dCheckValid(pos);
    this->perInstLock.Unlock(nullptr, 0);
}
