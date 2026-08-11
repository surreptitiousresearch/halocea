// ds::WEAK_PTR<entENTITY>::Release @0x823D39B8 — drop this handle's reference; free the handle
// once its refcount reaches 0; always null pHandle afterwards.
#include "../../headers/ws/ds/WEAK_PTR.h"

struct entENTITY; // boundary — only used as WEAK_PTR template argument (pointer-only)

extern "C" int osLockedDecrement(volatile int *slot);

template<>
void ds::WEAK_PTR<entENTITY>::Release()
{
    ds::WEAK_PTR_HANDLE<entENTITY> *handle = pHandle;
    if (handle) {
        osLockedDecrement(&handle->refCount);
        if (handle->refCount <= 0)
            operator delete(handle);
    }
    pHandle = nullptr;
}
