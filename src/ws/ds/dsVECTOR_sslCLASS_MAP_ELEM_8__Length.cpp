#include "dsVECTOR.h"
#include "../ssl/sslCLASS_MAP_ELEM.h"

// dsVECTOR<sslCLASS_MAP_ELEM,8>::Length @ 0x825C9AA0
// Number of live elements.
template<>
int dsVECTOR<sslCLASS_MAP_ELEM, 8>::Length() const
{
    return this->nElem;
}
