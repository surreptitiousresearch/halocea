#include "dsVECTOR.h"
#include "../ssl/sslCLASS_NAME_TO_IDX.h"

// dsVECTOR<sslCLASS::NAME_TO_IDX,8>::Compact @ 0x82A3B720
// Shrink the backing storage to exactly fit the live element count.
template<>
void dsVECTOR<ssl_class::NAME_TO_IDX, 8>::Compact()
{
    this->Realloc(this->nElem);
}
