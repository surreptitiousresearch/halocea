#include "../headers/hcex/haloENGINE_CONTROL.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<dsPAIR<int, void (haloENGINE_CONTROL::*)(unsigned long, const dsPARAM_LIST &)>, 8>::~dsVECTOR()
//     @ 0x823C2788
//
// Destructor for haloENGINE_CONTROL::GetEventMap()'s backing store. The element type
// (dsPAIR<int, member-fn-ptr>) is trivially destructible, so the per-element loop runs no teardown;
// only the backing storage is freed.
typedef dsPAIR<int, void (haloENGINE_CONTROL::*)(unsigned long, const dsPARAM_LIST &)> EventMapEntry;

template<>
dsVECTOR<EventMapEntry, 8>::~dsVECTOR()
{
    for (int index = 0; index < this->nElem; ++index)
        ; // trivially-destructible element type: no per-element teardown
    dlFree(this->pData);
}
