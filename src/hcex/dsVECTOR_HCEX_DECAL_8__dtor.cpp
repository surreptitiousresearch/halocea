// dsVECTOR<HCEX_DECAL,8>::~dsVECTOR() @ 0x823D5CA8
//
// Destroy the decal vector: run HCEX_DECAL::~HCEX_DECAL() on every live slot (its only non-trivial
// member is the ref-counted `name` string; pos is POD), then free the backing store. The
// disassembly shows the element dtor fully inlined (name.pBuffer refcount--/dlFree); calling the
// element destructor here reproduces that exactly. Reversed per-instantiation.

#include "../headers/hcex/HCEX_DECAL.h"
#include "../headers/ws/ds/dsVECTOR.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc free (unmangled C export)

template<>
dsVECTOR<HCEX_DECAL, 8>::~dsVECTOR()
{
    for (int i = 0; i < this->nElem; ++i)
        this->pData[i].~HCEX_DECAL();
    dlFree(this->pData);
}
