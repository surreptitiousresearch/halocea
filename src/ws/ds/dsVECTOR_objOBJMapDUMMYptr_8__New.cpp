#include "dsVECTOR.h"
#include "dsObjOBJVecMapDUMMY.h"

// New<...objOBJ-map DUMMY*>(place,val) @ 0x82681680
// Placement-"construct" helper used by dsVECTOR<T*,8>::Resize(size,fill)'s fill-callback for
// pointer element types (copy-assigns the fill pointer). Null `place` is a no-op passthrough.
template<class T>
T *New(T *place, const T *val);

template<>
dsObjOBJVecMapDUMMY **New<dsObjOBJVecMapDUMMY *>(dsObjOBJVecMapDUMMY **place, dsObjOBJVecMapDUMMY *const *val)
{
    if (!place)
        return nullptr;
    *place = *val;
    return place;
}
