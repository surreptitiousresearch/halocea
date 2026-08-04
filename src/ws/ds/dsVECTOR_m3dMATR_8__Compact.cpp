#include "dsVECTOR.h"
#include "../m3d/m3dMATR.h"

// dsVECTOR<m3dMATR,8>::Compact @ 0x823EE250
// Shrink the backing storage to exactly the live element count.
template<>
void dsVECTOR<m3dMATR, 8>::Compact()
{
    this->Realloc(this->nElem);
}
