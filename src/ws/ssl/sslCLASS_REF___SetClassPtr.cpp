#include "sslCLASS_REF.h"
#include "sslCLASS.h"

// Global atomic ref-count helpers; bodies not reversed here.
extern "C" long osLockedIncrement(volatile long *pValue); // boundary
extern "C" long osLockedDecrement(volatile long *pValue); // boundary

// 0x82523540 — sslCLASS_REF::_SetClassPtr. Retarget pClass to `pCls`: add a ref to the incoming
// class, release the old one, and destroy+free it if its ref-count reaches zero.
void sslCLASS_REF::_SetClassPtr(sslCLASS *pCls)
{
    if (pCls)
        osLockedIncrement((volatile long *)&pCls->refCount.refCount);
    if (pClass)
    {
        if (osLockedDecrement((volatile long *)&pClass->refCount.refCount) <= 0)
        {
            sslCLASS *oldClass = pClass;
            if (oldClass)
            {
                oldClass->~sslCLASS();
                operator delete(oldClass);
            }
        }
    }
    pClass = pCls;
}
