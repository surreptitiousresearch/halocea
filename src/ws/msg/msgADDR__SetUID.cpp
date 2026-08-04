#include "msgADDR.h"
#include "msgSYSTEM.h"

// msgADDR::SetUID @ 0x8255D640
// Store the unique id and (re)register this address with the global message system.
void msgADDR::SetUID(unsigned long uniqueID)
{
    this->uniqueID = uniqueID;
    msgSystem.AssignUID(this);
}
