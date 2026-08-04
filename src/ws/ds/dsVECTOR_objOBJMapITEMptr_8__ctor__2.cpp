#include "dsVECTOR.h"

struct dsObjOBJVecMapITEM; // boundary — ds::MAP<objOBJ*,dsVECTOR<objOBJ*,8>,...>::ITEM (opaque; pointer element)

// dsVECTOR<...objOBJ-map ITEM*,8>::dsVECTOR(int,const T&,apCL) @ 0x826894D0
// Default-init empty, then Resize(size,fill) to presize and fill every slot with `fill`.
template<>
dsVECTOR<dsObjOBJVecMapITEM *, 8>::dsVECTOR(int size, dsObjOBJVecMapITEM *const &fill, const apCL &cl)
    : pData(nullptr), nElem(0), allocated(0), __cl(cl)
{
    this->Resize(size, fill);
}
