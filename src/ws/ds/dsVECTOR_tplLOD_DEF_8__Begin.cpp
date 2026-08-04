#include "dsVECTOR.h"
#include "../anim/tplLOD_DEF.h"

// dsVECTOR<tplLOD_DEF,8>::Begin @ 0x82A7C690
// Pointer to the first element of the backing storage.
template<>
const tplLOD_DEF *dsVECTOR<tplLOD_DEF, 8>::Begin() const
{
    return this->pData;
}
