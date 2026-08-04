#include "sslCLASS_REF.h"
#include "sslCLASS.h"

extern "C" long osLockedIncrement(volatile long *pValue); // boundary
extern "C" long osLockedDecrement(volatile long *pValue); // boundary

// sslCLASS_REF::operator=(const sslCLASS_REF&) @ 0x82523910 — ref-counted assign.
// Adds a ref to the incoming class first (so self-assignment through a temporary still
// works), then releases (and, at zero, destroys) the previously-held class before adopting
// the new pointer.
sslCLASS_REF &sslCLASS_REF::operator=(const sslCLASS_REF &ref)
{
    sslCLASS *incoming = ref.pClass;
    if (ref.pClass)
        osLockedIncrement((volatile long *)&ref.pClass->refCount.refCount);
    if (this->pClass)
    {
        if (osLockedDecrement((volatile long *)&this->pClass->refCount.refCount) <= 0)
        {
            sslCLASS *oldClass = this->pClass;
            if (oldClass)
            {
                oldClass->~sslCLASS();
                operator delete(oldClass);
            }
        }
    }
    this->pClass = incoming;
    return *this;
}
