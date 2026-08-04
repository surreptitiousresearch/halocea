#include "dsVECTOR.h"

struct dsObjOBJVecMapITEM; // boundary — ds::MAP<objOBJ*,dsVECTOR<objOBJ*,8>,...>::ITEM (opaque; pointer element)

// dsVECTOR<...objOBJ-map ITEM*,8>::Clear @ 0x826811B0
// Pointer elements are trivially destructible, so the generic per-element destructor loop is
// empty and this collapses to resetting the live count.
template<>
void dsVECTOR<dsObjOBJVecMapITEM *, 8>::Clear()
{
    this->nElem = 0;
}
