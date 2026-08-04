// ds::WEAK_PTR<entENTITY>::~WEAK_PTR @0x823D56E8 — drop this weak reference: decrement the shared
// handle's refcount and, when it reaches zero, delete the handle; then null pHandle. (Mirrors the
// Release() tail that SetPtr also runs, but is emitted as the destructor body here.)
#include "../../headers/ws/ds/WEAK_PTR.h"
#include <new> // operator delete

struct entENTITY; // boundary — only the inherited weak handle matters here

extern "C" int osLockedDecrement(volatile int *slot); // boundary — atomic decrement primitive

template<>
ds::WEAK_PTR<entENTITY>::~WEAK_PTR()
{
    ds::WEAK_PTR_HANDLE<entENTITY> *handle = this->pHandle;
    if (this->pHandle) {
        osLockedDecrement(&handle->refCount);
        if (handle->refCount <= 0) {
            if (this->pHandle)
                operator delete(this->pHandle);
        }
    }
    this->pHandle = nullptr;
}
