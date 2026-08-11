/* HCEX_ANIM_MNG_FP::`vector deleting destructor' @0x823D0BF8 — run ~animMNG() (via the real
 * destructor), then conditionally operator-deletes `this` when bit 0 of `deleteFlags` is set
 * (compiler-generated `??_E` thunk). */

#include "../../headers/hcex/HCEX_ANIM_MNG_FP.h"

HCEX_ANIM_MNG_FP *HCEX_ANIM_MNG_FP::vectorDeletingDtor(unsigned char deleteFlags)
{
    this->~HCEX_ANIM_MNG_FP();
    if ( deleteFlags & 1 )
        operator delete(this);
    return this;
}
