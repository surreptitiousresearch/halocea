#include "dsVECTOR.h"
#include "WEAK_PTR.h"
#include <new> // operator delete

struct entENTITY; // boundary — ent entity; referenced only through the weak handle.

extern "C" long osLockedDecrement(volatile int *value); // boundary — atomic decrement primitive
extern "C" void dlFree(void *ptr);                       // boundary — dlmalloc allocator free

// dsVECTOR<ds::WEAK_PTR<entENTITY>,8>::~dsVECTOR @ 0x823ED860
// Destructor: release every live weak reference (drop one reference on each slot's shared handle,
// delete the handle when its count reaches zero, then null the slot), then free the backing store.
// Same per-element teardown as Clear(), but ends by releasing the storage rather than reusing it.
template<>
dsVECTOR<ds::WEAK_PTR<entENTITY>, 8>::~dsVECTOR()
{
    int releasedCount = 0;
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
            ++releasedCount;
            slots[index++].pHandle = nullptr;
        } while (releasedCount < this->nElem);
    }
    dlFree(this->pData);
}
