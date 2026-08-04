#include "dsVECTOR.h"
#include "../rend/rendSR_LAMP_DATA.h"

// dsVECTOR<rendSR_LAMP_DATA,8>::IsEmpty @ 0x82A925D0
// True when there are no live elements.
template<>
bool dsVECTOR<rendSR_LAMP_DATA, 8>::IsEmpty() const
{
    return this->nElem == 0;
}
