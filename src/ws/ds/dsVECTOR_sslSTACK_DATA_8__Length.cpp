#include "dsVECTOR.h"
#include "../ssl/sslSTACK_DATA.h"

// dsVECTOR<sslSTACK_DATA,8>::Length @ 0x82AD7AC0
template<>
int dsVECTOR<sslSTACK_DATA, 8>::Length() const
{
    return this->nElem;
}
