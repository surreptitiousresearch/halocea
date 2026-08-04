#include "../headers/ws/ds/dsVECTOR.h"
#include "../headers/hcex/HALO_CHANNEL.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc free (unmangled C export)

// dsVECTOR<HALO_CHANNEL,8>::~dsVECTOR() @ 0x836B6C90
// Destroy the channel table: run ~HALO_CHANNEL on each element, then free the backing store.
template<>
dsVECTOR<HALO_CHANNEL, 8>::~dsVECTOR()
{
    for (int i = 0; i < this->nElem; ++i)
        this->pData[i].~HALO_CHANNEL();
    dlFree(this->pData);
}
