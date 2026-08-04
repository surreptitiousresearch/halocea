#include "animINST.h"
#include "../scn/scnSCENE.h"
#include "../cdt/cdtCOLL_ANIM_SCN.h"

// animINST::SetIGNORE_CDT_ALWAYS @ 0x825E1888
// Flag the instance so it is always excluded from collision-animation, then invalidate any
// cached collision-anim data held for it by the owning scene.
void animINST::SetIGNORE_CDT_ALWAYS()
{
    scnSCENE *pScn = this->pScn;
    this->state |= 0x10;
    if (pScn) {
        cdtCOLL_ANIM_SCN *pCollAnim = pScn->pCollAnim;
        if (pCollAnim)
            pCollAnim->Invalidate(this, 1);
    }
}
