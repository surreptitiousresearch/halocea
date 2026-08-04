/* HCEX_ANIM_MNG_FP::~HCEX_ANIM_MNG_FP @0x823CE290 — thunk straight to ~animMNG(); sync_data is a
 * plain POD block with nothing to release. */

#include "../../headers/hcex/HCEX_ANIM_MNG_FP.h"

HCEX_ANIM_MNG_FP::~HCEX_ANIM_MNG_FP()
{
    // ~animMNG() runs implicitly via base-class destruction order.
}
