#include "dsVECTOR.h"
#include "../rend/rendSR_LAMP_DATA.h"

// dsVECTOR<rendSR_LAMP_DATA,8>::Clear @ 0x8252C018
// Destroy every live element and reset the count. rendSR_LAMP_DATA is a trivially destructible
// packed word, so the per-element destructor loop is empty and collapses to nElem=0.
template<>
void dsVECTOR<rendSR_LAMP_DATA, 8>::Clear()
{
    this->nElem = 0;
}
