// dsPAIR<dsTSTRING<char>,dsVECTOR<hcexCINE_INFO,8>>::operator=(const dsPAIR &that) @ 0x823B8980
//
// Copy-assign one listCine entry. The key dsTSTRING<char> assignment is inlined (release the old
// shared buffer, freeing it at a zero refcount, then adopt `that`'s and bump its refcount); the
// value dsVECTOR<hcexCINE_INFO,8> is assigned via its own operator=. The key rebind is guarded by a
// self-assignment check; the value assignment runs unconditionally (its own guard covers self).
// Reversed per-instantiation.

#include "../headers/hcex/hcexCINE_INFO.h"
#include "../headers/ws/ds/dsVECTOR.h"
#include "../headers/ws/ds/dsTSTRING.h"
#include "../headers/ws/ds/dsPAIR.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc free (unmangled C export)

typedef dsPAIR<dsTSTRING<char>, dsVECTOR<hcexCINE_INFO, 8> > hcex_cine_entry;

template<>
hcex_cine_entry &hcex_cine_entry::operator=(const hcex_cine_entry &that)
{
    if (&that != this)
    {
        dsTSTRING_BUF_HEADER<char> *pBuffer = this->key.pBuffer;
        if (pBuffer->refCount-- == 1)
            dlFree(pBuffer);
        this->key.pBuffer = that.key.pBuffer;
        ++this->key.pBuffer->refCount;
    }
    this->val = that.val;
    return *this;
}
