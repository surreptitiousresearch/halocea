#include "animINST.h"

// ?GetMatrL2W@animINST@@QAAXPAVm3dMATR@@@Z  (0x825E4E80) — copy the instance's cached
// local-to-world transform into `pMatL2W`. If the instance follows a tracker and its follow
// pose has not been resolved this frame (state2 & 8 == 0), validate it first.
void animINST::GetMatrL2W(m3dMATR *pMatL2W)
{
    if (this->followTracker && (this->state2 & 8) == 0)
        this->Validate(2048); // 0x800

    *pMatL2W = this->matrL2W; // decompiler expands this into eight 64-bit field copies
}
