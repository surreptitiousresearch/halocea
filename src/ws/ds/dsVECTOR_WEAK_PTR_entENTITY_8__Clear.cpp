#include "dsVECTOR.h"
#include "WEAK_PTR.h"
#include <new> // operator delete

struct entENTITY; // boundary — ent entity; referenced only through the weak handle.

extern "C" long osLockedDecrement(volatile int *value); // boundary — atomic decrement primitive

// dsVECTOR<ds::WEAK_PTR<entENTITY>,8>::Clear @ 0x823ED7E0
// Drop every live weak reference: for each slot, release one reference on its shared handle,
// delete the handle when its count reaches zero, then null the slot. Finally reset the count.
template<>
void dsVECTOR<ds::WEAK_PTR<entENTITY>, 8>::Clear()
{
    int clearedCount = 0;
    if (this->nElem > 0) {
        int index = 0;
        do {
            ds::WEAK_PTR<entENTITY> *slots = this->pData;
            ds::WEAK_PTR_HANDLE<entENTITY> *handle = this->pData[index].pHandle;
            if (handle) {
                osLockedDecrement(&handle->refCount);
                if (handle->refCount <= 0) {
                    ds::WEAK_PTR_HANDLE<entENTITY> *handleToFree = slots[index].pHandle;
                    if (handleToFree)
                        operator delete(handleToFree);
                }
            }
            ++clearedCount;
            slots[index++].pHandle = nullptr;
        } while (clearedCount < this->nElem);
    }
    this->nElem = 0;
}
