#include "../../headers/ws/ai/aiGOAL_BASE.h" // aiSPD
#include "../../headers/ws/ds/dsSTRID.h"

// aiSPD::Is @ 0x831852E8
// True when this speed descriptor is named (its interned name id is valid) and that name equals
// `_name` (interned-id comparison).
bool aiSPD::Is(const dsSTRID &_name) const
{
    if (!this->name.IsValid())
        return false;
    if (_name.id != this->name.id)
        return false;
    return true;
}
