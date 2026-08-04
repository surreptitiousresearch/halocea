#include "sslOBJ_REF.h"
#include "sslOBJECT.h"

extern "C" long osLockedIncrement(volatile long *pValue); // boundary
extern "C" long osLockedDecrement(volatile long *pValue); // boundary

// sslOBJ_REF::sslOBJ_REF(sslOBJECT*) @ 0x82523B00 — construct-from-raw-pointer ctor.
// Adopts `pObj`, bumping its refcount first; also drops (and, at zero, destroys) whatever
// `this->pObject` already held, reproduced faithfully from the decompile.
sslOBJ_REF::sslOBJ_REF(sslOBJECT *pObj)
{
    this->pObject = nullptr;
    if (pObj)
    {
        osLockedIncrement((volatile long *)&pObj->refCount.refCount);
        if (this->pObject)
        {
            if (osLockedDecrement((volatile long *)&this->pObject->refCount.refCount) <= 0)
            {
                sslOBJECT *oldObject = this->pObject;
                if (oldObject)
                {
                    oldObject->~sslOBJECT();
                    operator delete(oldObject);
                }
            }
        }
    }
    this->pObject = pObj;
}
