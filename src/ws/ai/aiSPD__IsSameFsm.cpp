// aiSPD::IsSameFsm — 0x8326A1D8 (?IsSameFsm@aiSPD@@QBA_NABV1@@Z)
// True when this descriptor shares the same FSM configuration as `other`.
#include "../../headers/ws/ai/aiGOAL_BASE.h"

bool aiSPD::IsSameFsm(const aiSPD &other) const
{
    if ( isInertiaSupported != other.isInertiaSupported )
        return false;
    if ( other.fsmEvent.id != fsmEvent.id )
        return false;
    if ( other.fsmTriggerEventOn.id != fsmTriggerEventOn.id )
        return false;
    if ( other.fsmTriggerEventOff.id != fsmTriggerEventOff.id )
        return false;
    return true;
}
