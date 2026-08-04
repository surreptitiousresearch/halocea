#include "sslSOURCE_REF.h"
#include "sslSOURCE.h"

// Global atomic ref-count helpers; bodies not reversed here.
extern "C" long osLockedIncrement(volatile long *pValue); // boundary
extern "C" long osLockedDecrement(volatile long *pValue); // boundary

// 0x82A37060 — sslSOURCE_REF::_SetSourcePtr. Retarget `source` to `p`: add a ref to the incoming
// source, release the old one, and destroy+free it (scalar deleting destructor) at zero.
void sslSOURCE_REF::_SetSourcePtr(sslSOURCE *p)
{
    if (p)
        osLockedIncrement((volatile long *)&p->refCount.refCount);
    if (this->source &&
        osLockedDecrement((volatile long *)&this->source->refCount.refCount) <= 0 &&
        this->source)
    {
        sslSOURCE *oldSource = this->source;
        oldSource->~sslSOURCE();
        operator delete(oldSource);
    }
    this->source = p;
}
