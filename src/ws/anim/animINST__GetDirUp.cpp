/* ?GetDirUp@animINST@@QAAXPAUm3dV@@@Z @0x825E1598 */
#include "animINST.h"
#include "../m3d/m3d_boundary.h"

// ?GetDirUp@animINST@@QAAXPAUm3dV@@@Z  (0x825E1598) — copy out the instance's world-space up
// direction (the Y basis row of the local-to-world transform), under the per-instance lock.
void animINST::GetDirUp(m3dV *dir)
{
    this->perInstLock.Lock(nullptr, 0);
    *dir = this->matrL2W.v[1];   // row 1 = up axis; m4dV sliced to its m3dV part
    _m3dCheckValid(dir);
    this->perInstLock.Unlock(nullptr, 0);
}
