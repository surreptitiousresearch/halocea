#include "sslSOURCE_REF.h"
#include "sslSOURCE.h"

// sslSOURCE_REF::sslSOURCE_REF(const sslSOURCE_REF&) @ 0x82A37130 — copy ctor.
// Delegates entirely to _SetSourcePtr (0x82A37060), which handles the refcount bump/adopt.
sslSOURCE_REF::sslSOURCE_REF(const sslSOURCE_REF &ref)
{
    this->source = nullptr;
    _SetSourcePtr(ref.source);
}
