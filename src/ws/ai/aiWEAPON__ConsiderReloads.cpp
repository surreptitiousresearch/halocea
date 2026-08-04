#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/prop/propWPN_AI_USABLE.h"
#include "../../headers/ws/ds/WEAK_PTR.h"
#include "../../headers/ws/ds/ds_new.h" // global operator delete (weak-handle free)

extern "C" long osLockedIncrement(volatile int *value); // boundary — atomic increment primitive
extern "C" long osLockedDecrement(volatile int *value); // boundary — atomic decrement primitive

// aiWEAPON::ConsiderReloads() @ 0x832B3850
// ?ConsiderReloads@aiWEAPON@@IAAXH@Z
//
// Run the instant-reload test over every registered weapon controller: for each live controller
// whose clip has dropped to/below `least_percent`, reload it instantly. Each controller is reached
// through its weak-ref handle, which is locked (refcount bumped) across the check and released
// afterwards — freeing the handle when its refcount hits zero.
void aiWEAPON::ConsiderReloads(int least_percent)
{
    int n = ctrlsList.nElem;
    for (int i = 0; i < n; ++i)
    {
        ds::WEAK_PTR_HANDLE<propWPN_AI_USABLE> *pHandle = ctrlsList[i].ctrl.pHandle;
        if (!pHandle)
            continue;

        osLockedIncrement(&pHandle->refCount); // lock the handle
        propWPN_AI_USABLE *ctrl = pHandle->pPtr;
        if (ctrl && !rld.IsLoaded(ctrl, least_percent))
            ReloadInstantly(ctrl);

        osLockedDecrement(&pHandle->refCount); // release the handle
        if (pHandle->refCount <= 0)
            operator delete(pHandle);
    }
}
