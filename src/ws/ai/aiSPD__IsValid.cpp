#include "../../headers/ws/ai/aiGOAL_BASE.h" // aiSPD
#include "../../headers/ws/ds/dsSTRID.h"

// aiSPD::IsValid @ 0x83185348
// True when both the speed name and its associated FSM event are valid interned ids.
bool aiSPD::IsValid() const
{
    if (!this->name.IsValid())
        return false;
    if (!this->fsmEvent.IsValid())
        return false;
    return true;
}
