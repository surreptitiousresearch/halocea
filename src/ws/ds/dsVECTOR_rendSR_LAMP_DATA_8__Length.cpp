#include "dsVECTOR.h"
#include "../rend/rendSR_LAMP_DATA.h"

// dsVECTOR<rendSR_LAMP_DATA,8>::Length @ 0x829EB980
// Number of live elements.
template<>
int dsVECTOR<rendSR_LAMP_DATA, 8>::Length() const
{
    return this->nElem;
}
