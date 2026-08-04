#include "dsVECTOR.h"
#include "../anim/tplLOD_DEF.h"

// dsVECTOR<tplLOD_DEF,8>::Length @ 0x82A7C688
// Number of live elements.
template<>
int dsVECTOR<tplLOD_DEF, 8>::Length() const
{
    return this->nElem;
}
