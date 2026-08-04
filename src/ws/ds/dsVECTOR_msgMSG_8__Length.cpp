#include "dsVECTOR.h"
#include "../msg/msgMSG.h"

// dsVECTOR<msgMSG,8>::Length @ 0x82AC3AB8
template<>
int dsVECTOR<msgMSG, 8>::Length() const
{
    return this->nElem;
}
