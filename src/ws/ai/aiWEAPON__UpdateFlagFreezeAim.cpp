#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/wpn/wpnS_DESC.h"

// aiWEAPON::UpdateFlagFreezeAim @ 0x832932E8
// ?UpdateFlagFreezeAim@aiWEAPON@@IAAX_N@Z
//
// Drive the mind's "freeze aim" bit (0x200) for strikes whose descriptor requests it: set it when
// enabling and the freeze-delay timer has elapsed; clear it when disabling.
void aiWEAPON::UpdateFlagFreezeAim(bool enable)
{
    const wpnS_DESC *desc = tieSelected.desc;
    if (desc && desc->isNeedToFreezeAim) {
        aiSTATUS *st = pBrain->st.pointee;
        if (enable) {
            if (st->aim.timerToFreezeAim < 0.000001f)
                st->mind.val |= 0x200;
        } else {
            st->mind.val &= ~0x200;
        }
    }
}
