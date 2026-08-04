#include "dsVECTOR.h"
#include "dsDATA.h"

// dsVECTOR<dsDATA,8>::Compact @ 0x827AF838
// Shrink the backing storage exactly to the live element count.
template<>
void dsVECTOR<dsDATA, 8>::Compact()
{
    this->Realloc(this->nElem);
}
