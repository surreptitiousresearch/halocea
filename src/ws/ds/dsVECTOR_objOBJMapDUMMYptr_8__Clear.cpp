#include "dsVECTOR.h"
#include "dsObjOBJVecMapDUMMY.h"

// dsVECTOR<...objOBJ-map DUMMY*,8>::Clear @ 0x826811E8
// Pointer elements are trivially destructible, so the generic per-element destructor loop is
// empty and this collapses to resetting the live count.
template<>
void dsVECTOR<dsObjOBJVecMapDUMMY *, 8>::Clear()
{
    this->nElem = 0;
}
