#include "dsVECTOR.h"
#include "../ap/apCOUNTER_UNIT.h"

// dsVECTOR<apCOUNTER_UNIT,8>::Length @ 0x82714118
template<>
int dsVECTOR<apCOUNTER_UNIT, 8>::Length() const
{
    return this->nElem;
}
