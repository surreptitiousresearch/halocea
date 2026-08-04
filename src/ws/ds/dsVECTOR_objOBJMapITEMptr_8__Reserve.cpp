#include "dsVECTOR.h"

struct dsObjOBJVecMapITEM; // boundary — ds::MAP<objOBJ*,dsVECTOR<objOBJ*,8>,...>::ITEM (opaque; pointer element)

// dsVECTOR<...objOBJ-map ITEM*,8>::Reserve @ 0x82688708
template<>
void dsVECTOR<dsObjOBJVecMapITEM *, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
