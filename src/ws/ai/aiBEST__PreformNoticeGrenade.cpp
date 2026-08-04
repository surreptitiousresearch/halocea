#include "../../headers/ws/ai/aiBEST.h"
#include "../../headers/ws/ds/ds_new.h" // tracked operator new / global operator delete

extern "C" long osLockedDecrement(volatile int *value); // boundary — atomic decrement primitive

// aiBEST::PreformNoticeGrenade @ 0x83297690
// ?PreformNoticeGrenade@aiBEST@@AAAXXZ
//
// Deliver a deferred grenade notice: if a live referent is still held by the delayed weak-ptr,
// release the weak handle (inlined WEAK_PTR::Release — decrement its refcount and free it once it
// hits zero), null the slot, then hand the grenade entity to the current top behaviour (unless it
// has finished).
void aiBEST::PreformNoticeGrenade()
{
    ds::WEAK_PTR_HANDLE<entENTITY> *pHandle = delayedNoticeGrenade.grenade.pHandle;
    if (!pHandle)
        return;

    entENTITY *pPtr = pHandle->pPtr;
    if (!pPtr)
        return;

    osLockedDecrement(&pHandle->refCount);
    if (pHandle->refCount <= 0) {
        if (delayedNoticeGrenade.grenade.pHandle)
            operator delete(delayedNoticeGrenade.grenade.pHandle);
    }
    delayedNoticeGrenade.grenade.pHandle = nullptr;

    bhvBEHAV *top = GetTopBehavior();
    if (top && (top->state.val & BHV_ST_FINISHED) == 0)
        top->NoticeGrenade(pPtr);
}
