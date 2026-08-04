#include "dsVECTOR.h"
#include "../ssl/sslCLASS_REF.h"

// dsVECTOR<sslCLASS_REF,8>::Length @ 0x825C9AB8
template<>
int dsVECTOR<sslCLASS_REF, 8>::Length() const
{
    return this->nElem;
}
