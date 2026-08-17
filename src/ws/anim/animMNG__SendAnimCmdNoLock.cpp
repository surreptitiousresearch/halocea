/* ?SendAnimCmdNoLock@animMNG@@QAAXABUanimCMD_BASE@@AAUanimUPDATE_STATUS@@@Z @0x8267C338 */
#include "animMNG.h"
#include "animLAYER_BASE.h"
#include "animCMD_BASE.h"
#include "animUPDATE_STATUS.h"

// ?SendAnimCmdNoLock@animMNG@@QAAXABUanimCMD_BASE@@AAUanimUPDATE_STATUS@@@Z  (0x8267C338) —
// dispatch `cmd` to its target blend layer(s) without taking the per-instance lock, accumulating
// the result in `updStatus`. GetDestLayer returns -2 (broadcast to every layer), a concrete layer
// index, or a negative "no target" value.
void animMNG::SendAnimCmdNoLock(const animCMD_BASE &cmd, animUPDATE_STATUS *updStatus)
{
    int destLayer = cmd.GetDestLayer(this);
    if (destLayer == -2)
    {
        for (int i = 0; i < this->layerList.nElem; ++i)
        {
            animLAYER_BASE *pLayer = this->layerList[i].val.pointee;
            pLayer->HandleCmd(&cmd, updStatus);
        }
    }
    else if (destLayer >= 0)
    {
        if (destLayer < this->layerList.nElem)
        {
            animLAYER_BASE *pLayer = this->layerList[destLayer].val.pointee;
            pLayer->HandleCmd(&cmd, updStatus);
        }
    }
}
