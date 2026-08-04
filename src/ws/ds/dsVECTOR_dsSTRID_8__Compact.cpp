#include "dsVECTOR.h"
#include "dsSTRID.h"

// dsVECTOR<dsSTRID,8>::Compact @ 0x823DC388
// Shrink the backing storage to exactly fit the live element count.
template<>
void dsVECTOR<dsSTRID, 8>::Compact()
{
    this->Realloc(this->nElem);
}
