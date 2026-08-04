#include "sslSOURCE_REF.h"
#include "sslSOURCE.h"

// sslSOURCE_REF::operator=(const sslSOURCE_REF&) @ 0x82A37170 — ref-counted assign.
// Delegates entirely to _SetSourcePtr (0x82A37060).
sslSOURCE_REF &sslSOURCE_REF::operator=(const sslSOURCE_REF &ref)
{
    _SetSourcePtr(ref.source);
    return *this;
}
