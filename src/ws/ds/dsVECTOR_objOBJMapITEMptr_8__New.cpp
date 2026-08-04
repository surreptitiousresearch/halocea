#include "dsVECTOR.h"

struct dsObjOBJVecMapITEM; // boundary — ds::MAP<objOBJ*,dsVECTOR<objOBJ*,8>,...>::ITEM (opaque; pointer element)

// New<...objOBJ-map ITEM*>(place,val) @ 0x82681310
// Placement-"construct" helper used by dsVECTOR<T*,8>::Resize(size,fill)'s fill-callback for
// pointer element types (the CallNew<U> sibling handles non-pointer/object element types,
// default-constructing in place; this one instead copy-assigns the fill pointer). A null `place`
// is a no-op passthrough (matches the generic Resize/Realloc null-check idiom).
template<class T>
T *New(T *place, const T *val);

// Explicit specialization for this element type (mirrors the DB's per-instantiation
// `New<dsObjOBJVecMapITEM*>` symbol).
template<>
dsObjOBJVecMapITEM **New<dsObjOBJVecMapITEM *>(dsObjOBJVecMapITEM **place, dsObjOBJVecMapITEM *const *val)
{
    if (!place)
        return nullptr;
    *place = *val;
    return place;
}
