#include "dsVECTOR.h"

struct dsObjOBJVecMapITEM; // boundary — ds::MAP<objOBJ*,dsVECTOR<objOBJ*,8>,...>::ITEM (opaque; pointer element)

// dsVECTOR<...objOBJ-map ITEM*,8>::Swap @ 0x82681508
// Exchange the backing store, element count, and capacity with another vector (three-field swap).
template<>
void dsVECTOR<dsObjOBJVecMapITEM *, 8>::Swap(dsVECTOR<dsObjOBJVecMapITEM *, 8> &other)
{
    dsObjOBJVecMapITEM **pData = this->pData;
    this->pData = other.pData;
    other.pData = pData;

    int nElem = this->nElem;
    this->nElem = other.nElem;
    other.nElem = nElem;

    int allocated = this->allocated;
    this->allocated = other.allocated;
    other.allocated = allocated;
}
