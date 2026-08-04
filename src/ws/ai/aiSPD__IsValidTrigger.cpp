#include "../../headers/ws/ai/aiGOAL_BASE.h" // aiSPD
#include "../../headers/ws/ds/dsSTRID.h"

// aiSPD::IsValidTrigger @ 0x831853A0
// A trigger pair is valid only when the on/off FSM trigger events are consistently specified:
// either both empty or both set. (The disassembly computes this as
// (!fsmTriggerEventOff.IsEmpty()) == (!fsmTriggerEventOn.IsEmpty()) via cntlzw of IsEmpty().)
bool aiSPD::IsValidTrigger() const
{
    bool onSet  = !this->fsmTriggerEventOn.IsEmpty();
    bool offSet = !this->fsmTriggerEventOff.IsEmpty();
    return offSet == onSet;
}
