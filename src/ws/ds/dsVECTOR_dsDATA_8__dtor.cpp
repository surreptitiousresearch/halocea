#include "dsVECTOR.h"
#include "dsDATA.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc free

// dsVECTOR<dsDATA,8>::~dsVECTOR @ 0x823C26D8
// Destroy every live element (dsDATA::~dsDATA dispatches the stored type's Destroy virtual when
// non-empty), then release the backing storage.
template<>
dsVECTOR<dsDATA, 8>::~dsVECTOR()
{
    for (int i = 0; i < this->nElem; ++i)
        this->pData[i].~dsDATA();
    dlFree(this->pData);
}
