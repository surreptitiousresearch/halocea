#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/hcex/HALO_SOUND_PARAMS.h"

extern "C" void dlFree(void *ptr); // boundary -- dlmalloc allocator free

// dsVECTOR<HALO_SOUND_PARAMS,8>::~dsVECTOR @ 0x836B722C
// Destroy each live params entry -- the only owned resource is its nested `replace` vector's
// backing store (REPLACE_INFO is trivially destructible, so ~HALO_SOUND_PARAMS reduces to
// dlFree(replace.pData)) -- then free this vector's own element storage.
template<>
dsVECTOR<HALO_SOUND_PARAMS, 8>::~dsVECTOR()
{
    for (int index = 0; index < this->nElem; ++index)
        dlFree(this->pData[index].replace.pData);
    dlFree(this->pData);
}
