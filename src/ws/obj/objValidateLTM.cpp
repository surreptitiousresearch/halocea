#include "objOBJ.h"
#include "../anim/animINST.h"
#include "../os/osLOCK.h"
#include "obj_free_functions.h"
// @0x82A72390  ?objValidateLTM@@YAXPAVobjOBJ@@@Z
// Lazily validate a node's LTM chain. If the owning animINST already has a CDT skeleton
// driving it (state bit 0x4000000 set and GetCDTSkeleton() non-null), the skeleton owns LTM
// validation, so defer to animINST::Validate(0x2000). Otherwise, if the instance isn't
// already IsValid(1), take its per-instance lock and run _objValidateLTM() under
// animINST::Validate(2), unless state2 bit 0x4 (LTM externally driven) is set.

void objValidateLTM(objOBJ *pObj)
{
    if (!pObj)
        return;

    animINST *pInst = pObj->pInst;
    if (!pInst)
        return;

    if ((pInst->state & 0x4000000) != 0 && pInst->GetCDTSkeleton()) {
        pInst->Validate(0x2000);
        return;
    }

    if (!pInst->IsValid(1)) {
        pInst->perInstLock.Lock(nullptr, 0);
        pInst->Validate(2);
        if ((pInst->state2 & 4) == 0)
            _objValidateLTM(pObj);
        pInst->perInstLock.Unlock(nullptr, 0);
    }
}
