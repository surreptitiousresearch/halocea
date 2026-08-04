#include "sslCLASS_REF.h"
#include "sslCLASS.h"

extern "C" long osLockedDecrement(volatile long *pValue); // boundary

// sslCLASS_REF::~sslCLASS_REF() @ 0x825234E0 — release the referenced class, destroying and
// freeing it if this was the last reference.
sslCLASS_REF::~sslCLASS_REF()
{
    sslCLASS *heldClass = this->pClass;
    if (heldClass && osLockedDecrement((volatile long *)&heldClass->refCount.refCount) <= 0)
    {
        sslCLASS *oldClass = this->pClass;
        if (oldClass)
        {
            oldClass->~sslCLASS();
            operator delete(oldClass);
        }
    }
}
