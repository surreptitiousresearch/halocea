#include "dsVECTOR.h"
#include "../anim/tplLOD_DEF.h"

// dsVECTOR<tplLOD_DEF,8>::IsEmpty @ 0x82B0CD20
// True when there are no live elements.
template<>
bool dsVECTOR<tplLOD_DEF, 8>::IsEmpty() const
{
    return this->nElem == 0;
}
