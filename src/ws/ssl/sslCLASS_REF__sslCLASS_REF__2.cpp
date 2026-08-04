#include "sslCLASS_REF.h"
#include "sslCLASS.h"

// Global atomic ref-count helpers; bodies not reversed here (see os/osLockedIncrement.cpp,
// os/osLockedDecrement.cpp for the real definitions).
extern "C" long osLockedIncrement(volatile long *pValue); // boundary
extern "C" long osLockedDecrement(volatile long *pValue); // boundary

// sslCLASS_REF::sslCLASS_REF(const sslCLASS_REF&) @ 0x825238A0 — copy ctor.
// Copies `ref`'s class pointer, bumping its refcount first. Also drops (and, at zero,
// destroys) whatever `this->pClass` already held -- reproduced faithfully even though `this`
// is freshly-constructed storage, matching the decompile exactly.
sslCLASS_REF::sslCLASS_REF(const sslCLASS_REF &ref)
{
    this->pClass = nullptr;
    sslCLASS *incoming = ref.pClass;
    if (ref.pClass)
    {
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
    }
    this->pClass = incoming;
}
