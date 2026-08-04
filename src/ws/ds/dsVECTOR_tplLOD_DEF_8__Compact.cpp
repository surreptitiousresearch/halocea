#include "dsVECTOR.h"
#include "../anim/tplLOD_DEF.h"

// dsVECTOR<tplLOD_DEF,8>::Compact @ 0x82B0DEA8
// Shrink the backing storage to exactly fit the live element count.
template<>
void dsVECTOR<tplLOD_DEF, 8>::Compact()
{
    this->Realloc(this->nElem);
}
