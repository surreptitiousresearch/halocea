#include "sslSOURCE_REF.h"
#include "sslSOURCE.h"

extern "C" long osLockedDecrement(volatile long *pValue); // boundary

// sslSOURCE_REF::~sslSOURCE_REF() @ 0x82A370D8 — release the referenced source. When this was
// the last reference, destroy and free it.
// DEVIATION: decompile calls the scalar deleting destructor (`sslSOURCE::\`scalar deleting
// destructor'(source, 1u)`, i.e. dtor + operator delete combined via the deleting-dtor flag);
// modeled here as the equivalent explicit dtor-then-delete pair, matching the sibling
// _SetSourcePtr.cpp convention for this same class.
sslSOURCE_REF::~sslSOURCE_REF()
{
    sslSOURCE *heldSource = this->source;
    if (heldSource && osLockedDecrement((volatile long *)&heldSource->refCount.refCount) <= 0)
    {
        sslSOURCE *oldSource = this->source;
        if (oldSource)
        {
            oldSource->~sslSOURCE();
            operator delete(oldSource);
        }
    }
}
