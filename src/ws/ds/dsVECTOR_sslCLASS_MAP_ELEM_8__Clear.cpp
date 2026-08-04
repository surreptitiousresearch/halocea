#include "dsVECTOR.h"
#include "../ssl/sslCLASS_MAP_ELEM.h"

// dsVECTOR<sslCLASS_MAP_ELEM,8>::Clear @ 0x825CA4E0
// Destroy every live element's cls ref (releasing the referenced script class) and reset the
// count. `name` (dsSTRID) is trivially destructible (nothing to release).
template<>
void dsVECTOR<sslCLASS_MAP_ELEM, 8>::Clear()
{
    for (int index = 0; index < this->nElem; ++index)
        this->pData[index].cls.~sslCLASS_REF();
    this->nElem = 0;
}
