// aiSPD::HaveTrigger — 0x8326A228 (?HaveTrigger@aiSPD@@QBA_NXZ)
// True when both the on and off FSM trigger events are valid interned ids.
#include "../../headers/ws/ai/aiGOAL_BASE.h"

bool aiSPD::HaveTrigger() const
{
    if ( !fsmTriggerEventOn.IsValid() )
        return false;
    if ( !fsmTriggerEventOff.IsValid() )
        return false;
    return true;
}
