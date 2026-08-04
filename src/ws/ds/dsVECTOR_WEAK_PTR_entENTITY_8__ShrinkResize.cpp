#include "dsVECTOR.h"
#include "WEAK_PTR.h"
#include "ds_assert_boundary.h"

struct entENTITY; // boundary — ent entity; referenced only through the weak handle.

extern "C" long osLockedDecrement(volatile int *value); // boundary — atomic decrement

// dsVECTOR<ds::WEAK_PTR<entENTITY>,8>::ShrinkResize @ 0x823D8898
// Shrink the live count to `size`, releasing each weak reference above the new count: atomically
// drop the shared handle's refcount, delete the handle once it reaches zero, and null the slot.
template<>
void dsVECTOR<ds::WEAK_PTR<entENTITY>, 8>::ShrinkResize(int size)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            270,
            "size", size);

    if (size < this->nElem) {
        for (int index = size; index < this->nElem; ++index) {
            ds::WEAK_PTR_HANDLE<entENTITY> *handle = this->pData[index].pHandle;
            if (handle) {
                osLockedDecrement(&handle->refCount);
                if (handle->refCount <= 0)
                    operator delete(this->pData[index].pHandle);
            }
            this->pData[index].pHandle = nullptr;
        }
        this->nElem = size;
    }
}
