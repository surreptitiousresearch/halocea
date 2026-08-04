// sslSOURCE::~sslSOURCE @0x82A37008 (body inlined into the scalar-deleting-dtor entry point; no
// separate call site was observed) — free Breakpoints.pData and validBPLines.pData
// unconditionally, then drop source's buffer ref (freeing it once the count hits 0).
#include "../../headers/ws/ssl/sslSOURCE.h"

extern "C" void dlFree(void *ptr);

sslSOURCE::~sslSOURCE()
{
    dlFree(Breakpoints.pData);
    dlFree(validBPLines.pData);
    if (--source.pBuffer->refCount == 0)
        dlFree(source.pBuffer);
}

// sslSOURCE::`scalar deleting destructor' @0x82A37008 — runs ~sslSOURCE(), then conditionally
// operator delete(this) when bit 0 of deleteFlags is set.
sslSOURCE *sslSOURCE::scalarDeletingDtor(unsigned char deleteFlags)
{
    this->~sslSOURCE();
    if (deleteFlags & 1)
        operator delete(this);
    return this;
}
