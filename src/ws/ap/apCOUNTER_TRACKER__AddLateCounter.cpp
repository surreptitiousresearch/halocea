#include "apCOUNTER_TRACKER.h"
#include "apCOUNTER.h"
#include "apCOUNTER_UNIT.h"

// apCOUNTER_TRACKER::AddLateCounter @ 0x82718398
void apCOUNTER_TRACKER::AddLateCounter(apCOUNTER *counter)
{
    for (int unitIndex = 0; unitIndex < counter->GetNUnits(0xFFu); )
    {
        apCOUNTER_UNIT &unit = counter->counterUnits[unitIndex++];
        unit.state.state |= 0x40u; // mark unit as late-added
    }
    this->cntAdded = true;
}
