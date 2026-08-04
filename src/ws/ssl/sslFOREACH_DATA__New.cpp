#include "sslFOREACH_DATA.h"
#include <new>

// Global placement copy-construct helper: declared as the primary template it specializes
// (same convention as New_apDATA_TRACKER_RECORD.cpp).
template<class T> T *New(T *place, const T *val);

// New<sslFOREACH_DATA> @ 0x82AF0420
// Placement-copy-construct helper: copy-construct the class-filter ref (ref-counted, boundary
// sslCLASS_REF copy ctor) and copy the plain lastObjIdx scalar. Returns `place` unchanged (null
// passthrough).
template<>
sslFOREACH_DATA *New<sslFOREACH_DATA>(sslFOREACH_DATA *place, const sslFOREACH_DATA *val)
{
    if (!place)
        return nullptr;

    new (&place->checkClass) sslCLASS_REF(val->checkClass);
    place->lastObjIdx = val->lastObjIdx;
    return place;
}
