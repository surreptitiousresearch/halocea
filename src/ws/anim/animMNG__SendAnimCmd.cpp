/* ?SendAnimCmd@animMNG@@QAAXABUanimCMD_BASE@@@Z @0x8267D8E0 */
#include "animMNG.h"
#include "animINST.h"
#include "animCMD_BASE.h"
#include "animUPDATE_STATUS.h"

// ?SendAnimCmd@animMNG@@QAAXABUanimCMD_BASE@@@Z  (0x8267D8E0) — dispatch `cmd` to the manager's
// instance under the per-instance lock, then invalidate the instance's pose if the command reported
// a change (status bit 0).
void animMNG::SendAnimCmd(const animCMD_BASE &cmd)
{
    animINST *pInst = this->pInst;
    animUPDATE_STATUS status;
    status.stateOut.state = 0;
    pInst->perInstLock.Lock(nullptr, 0);
    this->SendAnimCmdNoLock(cmd, &status);
    this->pInst->perInstLock.Unlock(nullptr, 0);
    if ((status.stateOut.state & 1) != 0)
        this->pInst->Invalidate(4098, 1);   // 0x1002
}
