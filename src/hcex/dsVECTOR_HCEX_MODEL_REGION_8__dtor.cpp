// dsVECTOR<HCEX_MODEL_REGION,8>::~dsVECTOR() @ 0x823D7840
//
// Destroy the model-region vector: run HCEX_MODEL_REGION::~HCEX_MODEL_REGION() on every live slot
// (releases the `name` string, destroys the objNames dsVECTOR<dsTSTRING>, frees objIds' POD store),
// then free the backing store. The disassembly shows name's release and objIds' teardown inlined
// while objNames' ~dsVECTOR remains a visible CALL; calling the element destructor here reproduces
// the full sequence. Reversed per-instantiation.

#include "../headers/hcex/HCEX_MODEL_REGION.h"
#include "../headers/ws/ds/dsVECTOR.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc free (unmangled C export)

template<>
dsVECTOR<HCEX_MODEL_REGION, 8>::~dsVECTOR()
{
    for (int i = 0; i < this->nElem; ++i)
        this->pData[i].~HCEX_MODEL_REGION();
    dlFree(this->pData);
}
