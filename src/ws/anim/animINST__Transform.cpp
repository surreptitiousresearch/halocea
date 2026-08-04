#include "animINST.h"

// ?Transform@animINST@@QAAXPBVm3dMATR@@H@Z  (0x825E28C8) — pre/post-multiply the instance's
// local-to-world transform by `pMTrans` (order selected by `oper`), under the per-instance lock,
// then invalidate the derived transform state.
void animINST::Transform(const m3dMATR *pMTrans, int oper)
{
    this->perInstLock.Lock(nullptr, 0);
    this->matrL2W.Transform(pMTrans, oper);
    this->perInstLock.Unlock(nullptr, 0);
    this->Invalidate(2048, 1);   // stateVld bit 0x800
}
