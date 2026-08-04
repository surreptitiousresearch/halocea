#include "dsVECTOR.h"
#include "../m3d/m3dMATR.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<m3dMATR,8>::~dsVECTOR @ 0x823ED768
// m3dMATR is a plain POD — no per-element destructor, just free the backing storage.
template<>
dsVECTOR<m3dMATR, 8>::~dsVECTOR()
{
    dlFree(this->pData);
}
