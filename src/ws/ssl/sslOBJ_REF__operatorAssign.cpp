#include "sslOBJ_REF.h"
#include "sslOBJECT.h"

extern "C" long osLockedIncrement(volatile long *pValue); // boundary
extern "C" long osLockedDecrement(volatile long *pValue); // boundary

// sslOBJ_REF::operator=(const sslOBJ_REF&) @ 0x82523B70 — ref-counted assign.
// Adds a ref to the incoming object first, then releases (and, at zero, destroys) the
// previously-held object before adopting the new pointer.
sslOBJ_REF &sslOBJ_REF::operator=(const sslOBJ_REF &ref)
{
    sslOBJECT *incoming = ref.pObject;
    if (ref.pObject)
        osLockedIncrement((volatile long *)&ref.pObject->refCount.refCount);
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
    this->pObject = incoming;
    return *this;
}
