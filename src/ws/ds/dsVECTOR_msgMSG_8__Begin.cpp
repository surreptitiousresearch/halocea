#include "dsVECTOR.h"
#include "../msg/msgMSG.h"

// dsVECTOR<msgMSG,8>::Begin @ 0x82AC3AC8
template<>
msgMSG *dsVECTOR<msgMSG, 8>::Begin()
{
    return this->pData;
}
