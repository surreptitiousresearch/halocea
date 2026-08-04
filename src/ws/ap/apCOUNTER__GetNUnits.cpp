#include "apCOUNTER.h"
#include "apCOUNTER_UNIT.h"

// apCOUNTER::GetNUnits @ 0x82715CE8
// Count the units whose state bits intersect `atState`. A unit flagged 0x40 ("late-added") is
// treated as if it also carried bit 0x02.
int apCOUNTER::GetNUnits(unsigned char atState) const
{
    int matchCount = 0;
    for (int unitIndex = 0; unitIndex < this->counterUnits.nElem; ++unitIndex)
    {
        unsigned char state = this->counterUnits[unitIndex].state.state;
        if ((state & 0x40) != 0)
            state |= 2u;
        if ((state & atState) != 0)
            ++matchCount;
    }
    return matchCount;
}
