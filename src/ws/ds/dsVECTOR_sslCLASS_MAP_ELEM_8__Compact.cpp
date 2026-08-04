#include "dsVECTOR.h"
#include "../ssl/sslCLASS_MAP_ELEM.h"

// dsVECTOR<sslCLASS_MAP_ELEM,8>::Compact @ 0x825CFB28
// Shrink the backing storage to exactly fit the live element count.
template<>
void dsVECTOR<sslCLASS_MAP_ELEM, 8>::Compact()
{
    this->Realloc(this->nElem);
}
