/* ?Invalidate@cdtCOLL_ANIM_SCN@@QAAXPAVanimINST@@H@Z @0x82A25C70 */
#include "../../headers/ws/cdt/cdtCOLL_ANIM_SCN.h"
#include "../../headers/ws/anim/animINST.h"
#include "../../headers/ws/m3d/m3dOBB.h"

// 0x82A25C70
void cdtCOLL_ANIM_SCN::Invalidate(animINST *pInst, int isDelete)
{
    // idAnimCdt == 0xFF is the "not yet classified" sentinel; classify once by OBB size.
    if ( (unsigned char)pInst->idAnimCdt == 0xFF )
    {
        pInst->idAnimCdt = 0;
        m3dOBB *obb = pInst->GetOBB(0);
        if ( obb && obb->GetSize() > 5.0f )
            pInst->idAnimCdt = 1;
    }

    if ( isDelete )
    {
        // Instance teardown: invalidate both scenes unconditionally.
        pCollAnimSmall->Invalidate(pInst->id, isDelete);
        pCollAnimLarge->Invalidate(pInst->id, isDelete);
    }
    else
    {
        cdtCOLL_ANIM_BASE_SCN *target = pInst->idAnimCdt ? pCollAnimLarge : pCollAnimSmall;
        target->Invalidate(pInst->id, isDelete); /* isDelete == 0 on this branch */
    }
}
