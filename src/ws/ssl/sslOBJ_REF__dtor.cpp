#include "sslOBJ_REF.h"
#include "sslOBJECT.h"

extern "C" long osLockedDecrement(volatile long *pValue); // boundary

// sslOBJ_REF::~sslOBJ_REF() @ 0x825236C8 — release the referenced object, destroying and
// freeing it if this was the last reference.
sslOBJ_REF::~sslOBJ_REF()
{
    sslOBJECT *heldObject = this->pObject;
    if (heldObject && osLockedDecrement((volatile long *)&heldObject->refCount.refCount) <= 0)
    {
        sslOBJECT *oldObject = this->pObject;
        if (oldObject)
        {
            oldObject->~sslOBJECT();
            operator delete(oldObject);
        }
    }
}
