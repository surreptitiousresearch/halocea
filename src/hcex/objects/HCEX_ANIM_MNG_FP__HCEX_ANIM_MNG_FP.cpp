/* HCEX_ANIM_MNG_FP::HCEX_ANIM_MNG_FP @0x823CE208 — delegate to animMNG(pInst, nullptr) (no owning
 * anim system for this first-person bridge manager), then install this class's vftable. */

#include "../../headers/hcex/HCEX_ANIM_MNG_FP.h"

HCEX_ANIM_MNG_FP_vtbl HCEX_ANIM_MNG_FP::vftable; // boundary storage — real contents not modeled

HCEX_ANIM_MNG_FP::HCEX_ANIM_MNG_FP(animINST *pInst) : animMNG(pInst, nullptr)
{
    __vftable = reinterpret_cast<animMNG_vtbl *>(&HCEX_ANIM_MNG_FP::vftable);
}
