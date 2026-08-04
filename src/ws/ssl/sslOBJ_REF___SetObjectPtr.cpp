#include "sslOBJ_REF.h"
#include "sslOBJECT.h"

// Global atomic ref-count helpers; bodies not reversed here.
extern "C" long osLockedIncrement(volatile long *pValue); // boundary
extern "C" long osLockedDecrement(volatile long *pValue); // boundary

// 0x82523728 — sslOBJ_REF::_SetObjectPtr. Retarget pObject to `pObj`: add a ref to the incoming
// object, release the old one, and destroy+free it if its ref-count reaches zero.
void sslOBJ_REF::_SetObjectPtr(sslOBJECT *pObj)
{
    if (pObj)
        osLockedIncrement((volatile long *)&pObj->refCount.refCount);
    if (pObject)
    {
        if (osLockedDecrement((volatile long *)&pObject->refCount.refCount) <= 0)
        {
            sslOBJECT *oldObject = pObject;
            if (oldObject)
            {
                oldObject->~sslOBJECT();
                operator delete(oldObject);
            }
        }
    }
    pObject = pObj;
}
