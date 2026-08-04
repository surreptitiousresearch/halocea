// dsVECTOR<HCEX_EFFECT_LOC,8>::~dsVECTOR() @ 0x823D5C10
//
// Destroy the effect-location vector: run HCEX_EFFECT_LOC::~HCEX_EFFECT_LOC() on every live slot
// (its only non-trivial member is the ref-counted `name` string; pos/vec are POD), then free the
// backing store. The disassembly shows the element dtor fully inlined (name.pBuffer refcount--/
// dlFree); calling the element destructor here reproduces that exactly. Reversed per-instantiation.

#include "../headers/hcex/HCEX_EFFECT_LOC.h"
#include "../headers/ws/ds/dsVECTOR.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc free (unmangled C export)

template<>
dsVECTOR<HCEX_EFFECT_LOC, 8>::~dsVECTOR()
{
    for (int i = 0; i < this->nElem; ++i)
        this->pData[i].~HCEX_EFFECT_LOC();
    dlFree(this->pData);
}
