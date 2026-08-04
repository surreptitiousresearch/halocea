#include "sslOBJ_REF.h"
#include "sslOBJECT.h"

extern "C" long osLockedIncrement(volatile long *pValue); // boundary
extern "C" long osLockedDecrement(volatile long *pValue); // boundary

// sslOBJ_REF::sslOBJ_REF(const sslOBJ_REF&) @ 0x82523A90 — copy ctor.
// Copies `ref`'s object pointer, bumping its refcount first; also drops (and, at zero,
// destroys) whatever `this->pObject` already held, reproduced faithfully from the decompile.
sslOBJ_REF::sslOBJ_REF(const sslOBJ_REF &ref)
{
    this->pObject = nullptr;
    sslOBJECT *incoming = ref.pObject;
    if (ref.pObject)
    {
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
    }
    this->pObject = incoming;
}
