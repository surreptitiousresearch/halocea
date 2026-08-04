#include "dsVECTOR.h"

struct dsObjOBJVecMapITEM; // boundary — ds::MAP<objOBJ*,dsVECTOR<objOBJ*,8>,...>::ITEM (opaque; pointer element)

// dsVECTOR<...objOBJ-map ITEM*,8>::IsEmpty @ 0x82681060
template<>
bool dsVECTOR<dsObjOBJVecMapITEM *, 8>::IsEmpty() const
{
    return this->nElem == 0;
}
