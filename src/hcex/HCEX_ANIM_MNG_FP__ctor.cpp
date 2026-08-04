/* HCEX_ANIM_MNG_FP::HCEX_ANIM_MNG_FP @0x823CE220 — chain to the base animMNG ctor with a null
 * animSYSTEM, then install this class's own vtable. */

#include "../headers/hcex/HCEX_ANIM_MNG_FP.h"

HCEX_ANIM_MNG_FP::HCEX_ANIM_MNG_FP(animINST *pInst) : animMNG(pInst, nullptr)
{
    this->__vftable = (animMNG_vtbl *)&HCEX_ANIM_MNG_FP::vftable; // base __vftable type
}
