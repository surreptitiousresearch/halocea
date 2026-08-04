// cdtREFINE::~cdtREFINE @0x823D6590 — release the weak-reference handle on destroy.
// If other observers still hold the handle (refCount > 0 after decrement) just null its
// back-pointer; otherwise free the handle. Mirrors the ds::WEAK_PTR_BASE teardown idiom.
#include "../../headers/ws/cdt/cdtREFINE.h"

// &cdtREFINE::`vftable' — emitted vtable symbol. boundary.
extern cdtREFINE_vtbl cdtREFINE__vftable;
// osLockedDecrement — atomic decrement PPC intrinsic wrapper. boundary.
extern void osLockedDecrement(int *pValue);

cdtREFINE::~cdtREFINE()
{
    __vftable = &cdtREFINE__vftable;

    ds::WEAK_PTR_HANDLE<cdtREFINE> *pHandle = weakSelf.pHandle;
    if (pHandle)
    {
        osLockedDecrement((int *)&pHandle->refCount); // refCount is volatile int; decl takes int*
        if (pHandle->refCount > 0)
        {
            weakSelf.pHandle->pPtr = nullptr;
        }
        else
        {
            if (weakSelf.pHandle)
            {
                operator delete(weakSelf.pHandle);
                weakSelf.pHandle = nullptr;
                return;
            }
        }
        weakSelf.pHandle = nullptr;
    }
}
