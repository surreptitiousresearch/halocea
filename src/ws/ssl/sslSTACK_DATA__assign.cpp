#include "sslSTACK_DATA.h"
#include "../ds/dsDATA.h"

// Global placement copy-construct helper (New<T>(dst, src)); body not reversed here (same
// declared-only convention as sslSTACK_DATA__ConvertToDsData.cpp).
template<class T> T *New(T *dst, const T *src); // boundary

// --- other-subsystem boundary (declared, not reversed here) ---
extern int osLockedIncrement(int *pValue); // os: atomic increment, returns the incremented count

// sslSTACK_DATA::operator=(const sslSTACK_DATA &val) @ 0x82AF0AA0
// Copy-assign the payload described by val->type:
//  - TYPE_DS_DATA: placement-copy-construct the in-place dsDATA held in the 12-byte `data` arm.
//  - TYPE_SSLOBJECT_REF / TYPE_SSLOBJECT_SUBELEM_REF: copy the object/class pointer pair, bumping
//    each ref count (osLockedIncrement) when non-null. TYPE_SSLOBJECT_SUBELEM_REF additionally
//    copies the sub-element index.
//  - everything else: a raw copy of the whole union (matches the decompiler's whole-`___u1`
//    struct assignment -- the union has no other ref-counted arms).
// NOTE: the decompiler shows `this->funcParamStack.idx` / `this->int_val` as the destinations for
// the object/class pointer copies; those are union-alias artifacts of OBJ_REF::pObj (@0x04) and
// OBJ_REF::pClass / SUB_ELEM_REF::pObjClass (@0x08) -- reconstructed here via the named arms.
sslSTACK_DATA &sslSTACK_DATA::operator=(const sslSTACK_DATA &val)
{
    TYPE type = val.type;
    this->type = val.type;

    if (type == TYPE_DS_DATA)
    {
        New<dsDATA>(reinterpret_cast<dsDATA *>(this->data), reinterpret_cast<const dsDATA *>(val.data));
    }
    else if (type == TYPE_SSLOBJECT_REF)
    {
        sslOBJECT *pObj = val.objRef.pObj;
        this->objRef.pObj = pObj;
        if (pObj)
            osLockedIncrement(reinterpret_cast<int *>(pObj));

        sslCLASS *pClass = val.objRef.pClass;
        this->objRef.pClass = pClass;
        if (pClass)
            osLockedIncrement(reinterpret_cast<int *>(pClass));
    }
    else if (type == TYPE_SSLOBJECT_SUBELEM_REF)
    {
        sslOBJECT *pObj = val.subElemRef.pObj;
        this->subElemRef.pObj = pObj;
        if (pObj)
            osLockedIncrement(reinterpret_cast<int *>(pObj));

        sslCLASS *pObjClass = val.subElemRef.pObjClass;
        this->subElemRef.pObjClass = pObjClass;
        if (pObjClass)
            osLockedIncrement(reinterpret_cast<int *>(pObjClass));

        this->subElemRef.idx = val.subElemRef.idx;
    }
    else
    {
        this->data[0] = val.data[0];
        this->data[1] = val.data[1];
        this->data[2] = val.data[2];
    }

    return *this;
}
