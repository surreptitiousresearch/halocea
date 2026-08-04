#include "dsVECTOR.h"
#include "../rend/rendSR_LAMP_DATA.h"

// dsVECTOR<rendSR_LAMP_DATA,8>::Compact @ 0x8252DFD8
// Shrink the backing storage to exactly fit the live element count.
template<>
void dsVECTOR<rendSR_LAMP_DATA, 8>::Compact()
{
    this->Realloc(this->nElem);
}
