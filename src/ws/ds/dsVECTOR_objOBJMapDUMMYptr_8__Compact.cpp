#include "dsVECTOR.h"
#include "dsObjOBJVecMapDUMMY.h"

// dsVECTOR<...objOBJ-map DUMMY*,8>::Compact @ 0x82688468
// Shrink the backing storage exactly to the live element count.
template<>
void dsVECTOR<dsObjOBJVecMapDUMMY *, 8>::Compact()
{
    this->Realloc(this->nElem);
}
