#include "msgADDR.h"

// msgADDR::SetPriority @ 0x8255BE78
// Set the dispatch priority. When the address is not registered for managed re-sorting
// (stateAddr bit 0x20 clear) the internal key is recomputed directly; otherwise it is queued
// for a priority re-sort on the message system.
void msgADDR::SetPriority(unsigned int priority)
{
    // Original tests (cntlzw(state & 0x20) & 0x20) != 0, i.e. bit 0x20 is clear.
    if ((this->stateAddr.state & 0x20) == 0)
    {
        this->priority = priority;
        this->priorityInt = 10 * this->priority;
    }
    else
    {
        this->priority = priority;
        this->ChangePriority();
    }
}
