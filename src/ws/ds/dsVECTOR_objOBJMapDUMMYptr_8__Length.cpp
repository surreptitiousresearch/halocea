#include "dsVECTOR.h"
#include "dsObjOBJVecMapDUMMY.h"

// dsVECTOR<...objOBJ-map DUMMY*,8>::Length @ 0x82681078
template<>
int dsVECTOR<dsObjOBJVecMapDUMMY *, 8>::Length() const
{
    return this->nElem;
}
