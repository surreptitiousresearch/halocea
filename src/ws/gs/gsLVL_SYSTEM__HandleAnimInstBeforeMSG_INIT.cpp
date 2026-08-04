#include "gsLVL_SYSTEM.h"
#include "../obj/objOBJ.h"

// 0x82739F48 — called after an animINST's INIT message is queued (before the message is
// actually dispatched): finish LOD setup, free data the model no longer needs after load,
// then pick the object's render pass based on state bit 0x40.
void gsLVL_SYSTEM::HandleAnimInstBeforeMSG_INIT(animINST *pInst)
{
    pInst->LODInit();
    pInst->pObj->FreeUnnecessaryData();

    if ( (pInst->state & 0x40) != 0 )
        pInst->pObj->SetRenderPassId(1);
    else
        pInst->pObj->SetRenderPassId(2);
}
